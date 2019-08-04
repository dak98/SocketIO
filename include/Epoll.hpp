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
    Epoll(Epoll const& epoll) = delete;
    Epoll(Epoll&& epoll);

    ~Epoll();

    Epoll& operator=(Epoll const& epoll) = delete;
    Epoll& operator=(Epoll&& epoll);
    
    void addFd(int const fd, uint32_t const events) const;
    void removeFd(int const fd) const;
    epoll_event getEvent() const;
}; // Epoll
    
} // SocketIO

#endif // SOCKETIO_EPOLL_HPP_
