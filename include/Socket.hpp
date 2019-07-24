#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <iostream>
#include <string>

namespace SocketIO {

class Socket {
private:
    int _sockfd;
public:
    Socket();
    explicit Socket(const int sockfd)
	: _sockfd{sockfd} {}
    Socket(const Socket& socket) = delete;
    Socket(Socket&& socket);	

    ~Socket();
    
    Socket& operator=(const Socket& socket) = delete;
    Socket& operator=(Socket&& socket);
    
    int getSockfd() const { return _sockfd; }    
    std::string toString() const {
	return "{domain=AF_INET,type=SOCK_STREAM,protocol=0}";
    }
    friend bool operator==(const Socket& socket1, const Socket& socket2) {
	return socket1.getSockfd() == socket2.getSockfd();
    }
    friend inline std::ostream& operator<<(std::ostream& stream, const Socket& socket) {
	return stream << socket.toString();
    }
};

} // SocketIO

#endif // SOCKETIO_SOCKET_HPP_
