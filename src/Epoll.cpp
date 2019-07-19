#include <cerrno>
#include <cstring>
#include <Epoll.hpp>
#include <exception>
#include <unistd.h>

namespace SocketIO {

Epoll::Epoll() {
    _epfd = epoll_create1(0);
    if (_epfd == -1)
	throw std::runtime_error{"An error occured while creating epoll: " +
		static_cast<std::string>(std::strerror(errno))};
}

Epoll::~Epoll() {
    close(_epfd);
}

void Epoll::addFd(const int fd, const uint32_t events) const {
    epoll_event event { events, { .fd = fd } };
    if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event) == -1)
	throw std::runtime_error{"An error occured while adding a descriptor to monitor: " +
		static_cast<std::string>(std::strerror(errno))};
}

void Epoll::removeFd(const int fd) const {
    epoll_event event { 0, { .fd = fd } };
    if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &event) == -1)
	throw std::runtime_error{"An error occured while removing a monitored descriptor: " +
		static_cast<std::string>(std::strerror(errno))};
}

epoll_event Epoll::getEvent() const {
    epoll_event events[1];
    if (epoll_wait(_epfd, events, 1, -1) == -1)
	throw std::runtime_error{"An error occured while waiting for an epoll event: " +
		static_cast<std::string>(std::strerror(errno))};
    return events[0];
}

inline std::string Epoll::toString() const {
    return "";
}

std::ostream& operator<<(std::ostream& stream, const Epoll& epoll) {
    return stream << epoll.toString();
}
    
} // SocketIO
