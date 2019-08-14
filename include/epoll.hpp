#ifndef SOCKETIO_EPOLL_HPP_
#define SOCKETIO_EPOLL_HPP_

#include <cstdint>
#include <vector>
#include <utility>

#include "socket.hpp"

namespace socket_io
{

class epoll
{
    using socket_data = std::pair<int, ip_protocol>;
public:
    /* 
     * Because of this struct we hide the usage of union and the undefined
     * behaviour that comes with it
     */
    struct event
    {
	uint32_t flags;
	socket caused_by;
    }; // event
    
    /*
     * @throws - std::runtime_error => an error occured while creating epoll
     */
    epoll();
        
    // Copy operations are deleted as they would splot the objects
    epoll(epoll const& other) = delete;
    epoll(epoll&& other) noexcept;

    ~epoll() noexcept;
    
    auto operator=(epoll const& other) -> epoll& = delete;
    auto operator=(epoll&& other) noexcept -> epoll&;    
    
    /*
     * @throws - std::runtime_error => an error occured while adding a fd
     */
    auto add(socket const& to_monitor, uint32_t const events) -> void;
    /*
     * If the socket is not being monitored, nothing happends
     *
     * @throws - std::runtime_error => an error occured while removing a fd
     */
    auto remove(socket const& to_stop_monitor) -> void;

    auto get_native_handle() const noexcept -> int { return handle; }

    /*
     * @throws - std::system_error => interupted by a signal
     *         - std::runtime_error => an error occured while acquiring an event
     */
    auto get_event() const -> event;
private:
    int handle;
    std::vector<socket_data> monitored_sockets;
}; // epoll
    
} // socket_io

#endif // SOCKETIO_EPOLL_HPP_
