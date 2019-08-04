#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <iostream>
#include <string>

namespace SocketIO
{

class Socket
{
private:
    int sockfd;
public:
    Socket();
    explicit Socket(int const sockfd)
	: sockfd{sockfd} {}
    Socket(Socket const& socket) = delete;
    Socket(Socket&& socket);	

    ~Socket();
    
    Socket& operator=(Socket const& socket) = delete;
    Socket& operator=(Socket&& socket);
    
    int getSockfd() const { return sockfd; }
    std::string toString() const
    {
	return "{domain=AF_INET,type=SOCK_STREAM,protocol=0}";
    }
    friend bool operator==(Socket const& socket1, Socket const& socket2)
    {
	return socket1.getSockfd() == socket2.getSockfd();
    }
    friend inline std::ostream& operator<<(std::ostream& stream, Socket const& socket)
    {
	return stream << socket.toString();
    }
};

} // SocketIO

#endif // SOCKETIO_SOCKET_HPP_
