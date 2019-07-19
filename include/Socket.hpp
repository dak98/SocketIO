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
    explicit Socket(const int sockfd) : _sockfd{sockfd} {}
    int getSockfd() const { return _sockfd; }
    ~Socket();
    std::string toString() const;
    bool operator==(const Socket& socket) const { return _sockfd == socket.getSockfd(); }
    friend std::ostream& operator<<(std::ostream& stream, const Socket& socket);
};

} // SocketIO

#endif // SOCKETIO_SOCKET_HPP_
