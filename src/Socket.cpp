#include <Socket.hpp>

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <stdexcept>	
#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace SocketIO {

Socket::Socket() {
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1)
	throw std::runtime_error("An error occured while creating a socket: " +
				 std::string{std::strerror(errno)});
}

Socket::Socket(Socket&& socket)
    : _sockfd{socket._sockfd} {
    socket._sockfd = -1;
}    
    
Socket::~Socket() {
    /*
     * Error check is not done here, as the OS generally still closes the file descriptor
     * even if close() fails
     */
    close(_sockfd);
}    

Socket& Socket::operator=(Socket&& socket) {    
    _sockfd = socket._sockfd;
    socket._sockfd = -1;
    return *this;
}
    
} // SocketIO
