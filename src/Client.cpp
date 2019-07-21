#include <Client.hpp>

#include <cerrno>
#include <Connection.hpp>
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
    send(Message{_socket.getSockfd(), UNIT_EXIT, "Client has been closed"});
    shutdown(_socket.getSockfd(), SHUT_RDWR);
}

Message Client::recv() {
    return SocketIO::recv(_socket.getSockfd());
    
}

void Client::send(const Message& message) const {
    SocketIO::send(message);
}

std::string Client::toString() const {
    return "";
}
    
std::ostream& operator<<(std::ostream& stream, const Client& client) {
    return stream << client.toString();
}
    
}; // SocketIO
