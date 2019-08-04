#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <iostream>
#include <string>

namespace socket_io
{

class socket
{
private:
    int sockfd;
public:
    socket();
    explicit socket(int const sockfd)
	: sockfd{sockfd} {}
    socket(Socket const& socket) = delete;
    socket(Socket&& socket);	

    ~socket();
    
    socket& operator=(socket const& socket) = delete;
    socket& operator=(socket&& socket);
    
    int get_sockfd() const { return sockfd; }
    std::string to_string() const
    {
	return "{domain=AF_INET,type=SOCK_STREAM,protocol=0}";
    }
    friend bool operator==(socket const& socket1, socket const& socket2)
    {
	return socket1.get_sockfd() == socket2.get_sockfd();
    }
    friend inline std::ostream& operator<<(std::ostream& stream, socket const& socket)
    {
	return stream << socket.to_string();
    }
};

} // socket_io

#endif // SOCKETIO_SOCKET_HPP_
