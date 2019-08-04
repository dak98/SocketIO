#include <Client.hpp>

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <stdexcept>	
#include <sys/types.h>

#include <Connection.hpp>

#ifdef SOCKETIO_DEBUG
#include <Logger.hpp>
#endif

namespace SocketIO
{

Client::Client(Address const& addressOfServer)
    : server{addressOfServer}
{
    sockaddr_in const addr = server.getAddr();
    if (connect(socket.getSockfd(), reinterpret_cast<const sockaddr*>(&addr), sizeof addr) == -1)
	throw std::runtime_error{"An error occured while connecting to the server: " +
		static_cast<std::string>(std::strerror(errno))};
    Message msg = recv();
    if (msg.getMessageType() != INIT_MSG)
	throw std::runtime_error{"Did not receive INIT_MSG from the server"}; 
    id = msg.getId();
    send({id, INIT_MSG, "init"});
    #ifdef SOCKETIO_DEBUG
    initLogging();
    BOOST_LOG_TRIVIAL(info) << "[CLIENT] Started the client";
    #endif
}

Client::~Client()
{
    send({id, UNIT_EXIT, "Client has closed the connection"});
    shutdown(socket.getSockfd(), SHUT_RDWR);
    #ifdef SOCKETIO_DEBUG
    initLogging();
    BOOST_LOG_TRIVIAL(info) << "[CLIENT] Closed the client";
    #endif
}

Message Client::recv()
{
    return SocketIO::recv(socket.getSockfd());
    #ifdef SOCKETIO_DEBUG
    initLogging();
    BOOST_LOG_TRIVIAL(info) << "[CLIENT] Received a message from the server";
    #endif
}

void Client::send(Message const& message) const
{
    SocketIO::send(socket.getSockfd(), message);
    #ifdef SOCKETIO_DEBUG
    initLogging();
    BOOST_LOG_TRIVIAL(info) << "[CLIENT] Sent a message to the server";
    #endif
}

std::string Client::toString() const
{
    std::string info;
    info += "{\n";
    info += "  socket=" + socket.toString() + "\n";
    info += "  server=" + server.toString() + "\n";
    info += "}";
    return info;
}
    
std::ostream& operator<<(std::ostream& stream, Client const& client)
{
    return stream << client.toString();
}
    
}; // SocketIO
