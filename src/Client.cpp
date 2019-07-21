#include <Client.hpp>

#include <cerrno>
#include <cstring>
#include <exception>
#include <sys/socket.h>
#include <sys/types.h>

namespace SocketIO {

Client::Client(const Address& addressOfServer)
    : _addressOfServer{addressOfServer} {
    const sockaddr_in addr = _addressOfServer.getAddr();
    if (connect(_socket.getSockfd(), reinterpret_cast<const sockaddr*>(&addr), sizeof addr) == -1)
	throw std::runtime_error{"An error occured while connecting to the server: " +
		static_cast<std::string>(std::strerror(errno))};
}

Client::~Client() {
    // TODO: Send a message to the server
    shutdown(_socket.getSockfd(), SHUT_RDWR);
}

Message Client::recv() {
    Message message;
    ssize_t length = ::recv(_socket.getSockfd(), reinterpret_cast<void*>(&message), sizeof(Message), MSG_NOSIGNAL);
    if (length == -1)
	throw std::runtime_error{"An error occured while reading a message: " +
		static_cast<std::string>(std::strerror(errno))};
    return message;
}

void Client::send(const Message& message) const {
    ssize_t length = ::send(_socket.getSockfd(), reinterpret_cast<const void*>(&message), sizeof(Message), MSG_NOSIGNAL);
    if (length == -1)
	throw std::runtime_error{"An error occured while sending a message: " +
		static_cast<std::string>(std::strerror(errno))};
}

std::string Client::toString() const {
    return "";
}
    
std::ostream& operator<<(std::ostream& stream, const Client& client) {
    return stream << client.toString();
}
    
}; // SocketIO
