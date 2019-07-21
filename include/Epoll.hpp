#ifndef SOCKETIO_EPOLL_HPP_
#define SOCKETIO_EPOLL_HPP_

#include <cstdint>
#include <sys/epoll.h>

namespace SocketIO {

class Epoll {
private:
    int _epfd;
public:
    Epoll();
    ~Epoll();
    void addFd(const int fd, const uint32_t events) const;
    void removeFd(const int fd) const;
    epoll_event getEvent() const;
}; // Epoll
    
} // SocketIO

#endif // SOCKETIO_EPOLL_HPP_
