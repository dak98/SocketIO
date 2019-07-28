#include <Client.hpp>

#include <cerrno>
#include <Connection.hpp>
#include <cstring>
#include <sys/socket.h>
#include <stdexcept>	
#include <sys/types.h>

#ifdef SOCKETIO_DEBUG
#include <Logger.hpp>
#endif

namespace SocketIO
{

Client::Client(const Address& addressOfServer)
    : server{addressOfServer}
{
    const sockaddr_in addr = server.getAddr();
    if (connect(socket.getSockfd(), reinterpret_cast<const sockaddr*>(&addr), sizeof addr) == -1)
	throw std::runtime_error{"An error occured while connecting to the server: " +
		static_cast<std::string>(std::strerror(errno))};
    #ifdef SOCKETIO_DEBUG
    initLogging();
    BOOST_LOG_TRIVIAL(info) << "[CLIENT] Started the client";
    #endif
}

Client::~Client()
{
    send(Message{socket.getSockfd(), UNIT_EXIT, "Client has been closed"});
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

void Client::send(const Message& message) const
{
    SocketIO::send(message);
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
    
std::ostream& operator<<(std::ostream& stream, const Client& client)
{
    return stream << client.toString();
}
    
}; // SocketIO
