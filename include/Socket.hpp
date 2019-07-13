#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <iostream>
#include <string>

namespace SocketIO {

class Socket {
private:
    int _sockfd;
public:
    explicit Socket();
    int getSockFd() const { return _sockfd; }
    ~Socket();
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& stream, const Socket& socket);
};

} // SocketIO

#endif // SOCKETIO_SOCKET_HPP_
