#include <epoll.hpp>

#include <algorithm>
#include <sys/epoll.h>
#include <stdexcept>
#include <system_error>
#include <unistd.h>

#include <utils.hpp>

#define EPFD_AFTER_MOVE 1

namespace socket_io
{

epoll::epoll(){
    handle = epoll_create1(0);
    if (handle == -1)
	throw std::runtime_error{"An error occured while creating epoll: " +
		                 get_errno_as_string()};
}

epoll::epoll(epoll&& other) noexcept
    : handle{other.handle}
{
    other.handle = EPFD_AFTER_MOVE;
}

epoll::~epoll() noexcept
{
    // Target of a move operation is responsible for closing the descriptor        
    if (handle != EPFD_AFTER_MOVE)
	close(handle);
}

auto epoll::operator=(epoll&& other) noexcept -> epoll&
{
    if (this == &other)
	return *this;
    
    handle = other.handle;
    other.handle = EPFD_AFTER_MOVE;
    return *this;
}

auto epoll::add(socket const& to_monitor, uint32_t const events) -> void
{
    int const sockfd = to_monitor.get_native_handle();
    epoll_event config { events, { .fd = sockfd } };
    if (epoll_ctl(handle, EPOLL_CTL_ADD, sockfd, &config) == -1)
	throw std::runtime_error{"An error occured while adding a fd: " +
		                 get_errno_as_string()};
    monitored_sockets.push_back({sockfd, to_monitor.get_ip_protocol()});
}

auto epoll::remove(socket const& to_stop_monitor) -> void
{
    int const sockfd = to_stop_monitor.get_native_handle();
    epoll_event config { 0, { .fd = sockfd } };
    if (epoll_ctl(handle, EPOLL_CTL_DEL, sockfd, &config) == -1)
    {
	if (errno == EBADF)
	    return;	
	throw std::runtime_error{"An error occured while removing a fd: " +
		                 get_errno_as_string()};
    }
    
    auto new_end = std::remove_if(std::begin(monitored_sockets),
				  std::end(monitored_sockets),
				  [=](socket_data const& data)
				  { return data.first == sockfd; });
    monitored_sockets.erase(new_end, std::end(monitored_sockets));		   
}

auto epoll::get_event() const -> event
{
    epoll_event events[1];
    int error_code = epoll_wait(handle, events, 1, -1);
    if (error_code == -1 && errno == EINTR)
	throw std::system_error{std::make_error_code(std::errc::interrupted)};
    if (error_code == -1)
	throw std::runtime_error{"An error occured while acquiring an event: " +
		                 get_errno_as_string()};

    auto it = std::find_if(std::begin(monitored_sockets),
			   std::end(monitored_sockets),
			   [=](socket_data const& data)
			   { return data.first == events[0].data.fd; });
    return {events[0].events, socket{it->first, it->second}};
}
    
} // socket_io
