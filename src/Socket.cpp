#include <Socket.hpp>

#include <cerrno>
#include <cstring>
#include <exception>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace SocketIO {

Socket::Socket() {
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1)
	throw std::runtime_error("An error occured while creating a socket: " +
				 std::string{std::strerror(errno)});
}

Socket::~Socket() {
    /*
     * Error check is not done here, as the OS generally still closes the file descriptor
     * even if close() fails
     */
    close(_sockfd);
}

inline std::string Socket::toString() const {
    return "[domain=AF_INET,type=SOCK_STREAM,protocol=0]";
}

std::ostream& operator<<(std::ostream& stream, const Socket& socket) {
    return stream << socket.toString();
}    

} // SocketIO
