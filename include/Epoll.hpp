#ifndef SOCKETIO_EPOLL_HPP_
#define SOCKETIO_EPOLL_HPP_

#include <cstdint>
#include <sys/epoll.h>

namespace SocketIO
{

class Epoll
{
private:
    int epfd;
public:
    Epoll();
    Epoll(const Epoll& epoll) = delete;
    Epoll(Epoll&& epoll);

    ~Epoll();

    Epoll& operator=(const Epoll& epoll) = delete;
    Epoll& operator=(Epoll&& epoll);
    
    void addFd(const int fd, const uint32_t events) const;
    void removeFd(const int fd) const;
    epoll_event getEvent() const;
}; // Epoll
    
} // SocketIO

#endif // SOCKETIO_EPOLL_HPP_
