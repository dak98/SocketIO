#include <Server.hpp>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <limits>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdexcept>	
#include <sys/types.h>
#include <unistd.h>

#include <Connection.hpp>

#ifdef SOCKETIO_DEBUG
#include <Logger.hpp>
#endif

namespace SocketIO
{

Server::Server(const int port)
{
    address = Address{INADDR_ANY, port};
    const sockaddr_in addr = address.getAddr();
    if (bind(socket.getSockfd(), reinterpret_cast<const sockaddr*>(&addr), sizeof addr) == -1)
	throw std::runtime_error{"An error occured while binding a socket: " +
		static_cast<std::string>(std::strerror(errno))};
}

Server::~Server()
{
    std::vector<int> sockfds = registry.getSockfds();
    for (const int sockfd : sockfds)
    {
	send({registry.getIdBySockfd(sockfd), UNIT_EXIT, "Server has been closed"});
	shutdown(sockfd, SHUT_RDWR);
    }
    shutdown(socket.getSockfd(), SHUT_RDWR);
    pthread_cancel(acceptClientsTID.native_handle());
    #ifdef SOCKETIO_DEBUG
    BOOST_LOG_TRIVIAL(info) << "[SERVER] Closed the server" << std::endl;
    #endif
}

void Server::acceptClients()
{
    for (;;)
    {
	int sockfd = accept(socket.getSockfd(), nullptr, nullptr);
	if (sockfd != -1) {
	    registry.add(sockfd);   
	    send({registry.getIdBySockfd(sockfd), INIT_MSG, "init"});
	    epoll.addFd(sockfd, EPOLLIN);
	    #ifdef SOCKETIO_DEBUG
	    BOOST_LOG_TRIVIAL(info) << "[SERVER] A new client with id"
				    << std::to_string(registry.getIdBySockfd(sockfd))
				    << " has connected" << std::endl;
	    #endif
	}
        #ifdef SOCKETIO_DEBUG
	else
	    BOOST_LOG_TRIVIAL(error) << "[SERVER] An attempt to accept a client failed"
				     << std::endl;
        #endif
    }
}

void Server::launch()
{
    if (listen(socket.getSockfd(), std::numeric_limits<int>::max()) == -1)
	throw std::runtime_error{"An error occured while starting the server: " +
		static_cast<std::string>(std::strerror(errno))};
    acceptClientsTID = std::thread(&Server::acceptClients, this);
    acceptClientsTID.detach();
    #ifdef SOCKETIO_DEBUG
    initLogging();
    BOOST_LOG_TRIVIAL(info) << "[SERVER] Started the server" << std::endl;
    #endif
}

Message Server::recv()
{
    auto&& [events, data] = epoll.getEvent();
    const int sockfd = data.fd;
    
    Message message = SocketIO::recv(sockfd);
    if (message.getMessageType() == UNIT_EXIT) // Client has closed its communication
    {
	registry.removeById(message.getId());
	epoll.removeFd(sockfd);
        #ifdef SOCKETIO_DEBUG
	BOOST_LOG_TRIVIAL(info) << "[SERVER] Client " + std::to_string(message.getId())
				<< " has disconnected" << std::endl;
        #endif
    }
    #ifdef SOCKETIO_DEBUG
    BOOST_LOG_TRIVIAL(info) << "[SERVER] Received a new message from client "
			    << std::to_string(id) << std::endl;
    #endif
    
    return message;
}

void Server::send(const Message& message) const
{
    const int sockfd = registry.getSockfdById(message.getId());
    SocketIO::send(sockfd, message);
    #ifdef SOCKETIO_DEBUG
    BOOST_LOG_TRIVIAL(info) << "[SERVER] Sent a message to client "
			    << std::to_string(message.getId()) << std::endl;
    #endif
}

std::string Server::toString() const
{
    return address.toString();
}

std::ostream& operator<<(std::ostream& stream, const Server& server)
{
    return stream << server.toString();
}

} // SocketIO
