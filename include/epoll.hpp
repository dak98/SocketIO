#ifndef SOCKETIO_EPOLL_HPP_
#define SOCKETIO_EPOLL_HPP_

#include <cstdint>
#include <sys/epoll.h>

namespace socket_io
{

class epoll
{
private:
    int epfd;
public:
    epoll();
    ~epoll();
        
    /*
     * Copy operations are deleted because both file descriptors would refer to
     * the same epoll object
     */       
    epoll(epoll const& epoll) = delete;
    epoll& operator=(epoll const& epoll) = delete;

    epoll(epoll&& epoll) noexcept;
    epoll& operator=(epoll&& epoll) noexcept;            
    
    void add(int const fd, uint32_t const events) const;
    void remove(int const fd) const;
    epoll_event get_event() const;
}; // epoll
    
} // socket_io

#endif // SOCKETIO_EPOLL_HPP_
