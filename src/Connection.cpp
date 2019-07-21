#include <Connection.hpp>

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <stdexcept>
#include <sys/types.h>

namespace SocketIO {

Message recv(const int fd) {
    Message message;
    ssize_t length = ::recv(fd, reinterpret_cast<void*>(&message), sizeof(Message), MSG_NOSIGNAL);
    if (length == 0)
	return Message{0, UNIT_EXIT, ""};
    if (length == -1)
	throw std::runtime_error{"An error occured while reading a message: " +
		static_cast<std::string>(std::strerror(errno))};
    return message;
}

void send(const Message& message) {
    ssize_t length = ::send(message.getSockfd(), reinterpret_cast<const void*>(&message), sizeof(Message), MSG_NOSIGNAL);
    if (length == -1)
	throw std::runtime_error{"An error occured while sending a message: " +
		static_cast<std::string>(std::strerror(errno))};
}
    
} // SocketIO
