#include <Epoll.hpp>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <unistd.h>

namespace SocketIO
{

Epoll::Epoll(){
    epfd = epoll_create1(0);
    if (epfd == -1)
	throw std::runtime_error{"An error occured while creating epoll: " +
		static_cast<std::string>(std::strerror(errno))};
}

Epoll::Epoll(Epoll&& epoll)
    : epfd{epoll.epfd}
{
    epoll.epfd = -1;
}

Epoll::~Epoll()
{
    close(epfd);
}

Epoll& Epoll::operator=(Epoll&& epoll)
{
    epfd = epoll.epfd;
    epoll.epfd = -1;
    return *this;
}

void Epoll::addFd(int const fd, uint32_t const events) const
{
    epoll_event event { events, { .fd = fd } };
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) == -1)
	throw std::runtime_error{"An error occured while adding a descriptor to monitor: " +
		static_cast<std::string>(std::strerror(errno))};
}

void Epoll::removeFd(int const fd) const
{
    epoll_event event { 0, { .fd = fd } };
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event) == -1)
	throw std::runtime_error{"An error occured while removing a monitored descriptor: " +
		static_cast<std::string>(std::strerror(errno))};
}

epoll_event Epoll::getEvent() const
{
    epoll_event events[1];
    if (epoll_wait(epfd, events, 1, -1) == -1)
	throw std::runtime_error{"An error occured while waiting for an epoll event: " +
		static_cast<std::string>(std::strerror(errno))};
    return events[0];
}
    
} // SocketIO
