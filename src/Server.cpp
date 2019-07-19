#include <Server.hpp>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <exception>
#include <limits>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace SocketIO {

Server::Server(const int port) {
    _address = Address{port};
    const sockaddr_in addr = _address.getAddr();
    if (bind(_socket.getSockfd(), reinterpret_cast<const sockaddr*>(&addr), sizeof addr) == -1)
	throw std::runtime_error{"An error occured while binding a socket: " +
		static_cast<std::string>(std::strerror(errno))};
}

Server::~Server() {
    for (const auto& client : _clients) {
	send(Message(client.getSockfd(), SERVER_EXIT, "Server has been closed"));
	shutdown(client.getSockfd(), SHUT_RDWR);
    }
    shutdown(_socket.getSockfd(), SHUT_RDWR);
    pthread_cancel(_acceptClientsTID.native_handle());
}

void Server::_acceptClients() {
    for (;;) {
	int sockfd = accept(_socket.getSockfd(), nullptr, nullptr);
	if (sockfd != -1) {
	    _epoll.addFd(sockfd, EPOLLIN);
	    _clients.emplace_back(Socket{sockfd}); // Add a log
	}
    }
}

void Server::launch() {
    if (listen(_socket.getSockfd(), std::numeric_limits<int>::max()) == -1)
	throw std::runtime_error{"An error occured while starting the server: " +
		static_cast<std::string>(std::strerror(errno))};
    _acceptClientsTID = std::thread(&Server::_acceptClients, this);
}

Message Server::recv() {
    auto&& [events, data] = _epoll.getEvent();
    Message message;
    ssize_t length = ::recv(data.fd, reinterpret_cast<void*>(&message), sizeof(Message), MSG_NOSIGNAL);
    if (length == 0) { // Client has closed its communication.
	auto newEnd = std::remove(std::begin(_clients), std::end(_clients), Socket{data.fd});
	_clients.erase(newEnd, std::end(_clients));
	_clients.shrink_to_fit();
	_epoll.removeFd(data.fd);
    }
    if (length == -1)
	throw std::runtime_error{"An error occured while reading a message: " +
		static_cast<std::string>(std::strerror(errno))};
    return message;
}

void Server::send(const Message& message) const {
    ssize_t length = ::send(message.getSockfd(), reinterpret_cast<const void*>(&message), sizeof(Message), MSG_NOSIGNAL);
    if (length == -1)
	throw std::runtime_error{"An error occured while sending a message: " +
		static_cast<std::string>(std::strerror(errno))};
}

inline std::string Server::toString() const {
    return "";
}

std::ostream& operator<<(std::ostream& stream, const Server& server) {
    return stream << server.toString();
}

} // SocketIO
