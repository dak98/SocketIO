#include <socket.hpp>

#include <cerrno>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils.hpp>

#define SOCKFD_AFTER_MOVE 1

namespace socket_io
{

socket::socket(ip_protocol const& ip_version)
    : ip_version{ip_version}
{
    handle = ::socket(static_cast<int>(ip_version), SOCK_STREAM, 0);
    if (handle == -1)
	throw std::runtime_error("An error occured while creating a socket: " +
				 get_errno_as_string());
}

socket::socket(int const sockfd, ip_protocol const& ip_version)
{
    socklen_t addrlen;
    sockaddr* addr;
    if (ip_version == ip_protocol::IPv4)
    {		    
        addrlen = sizeof(sockaddr_in);
	sockaddr_in addr_in;
	addr = reinterpret_cast<sockaddr*>(&addr_in);
    }
    else
    {
        addrlen = sizeof(sockaddr_in);
	sockaddr_in6 addr_in6;
	addr = reinterpret_cast<sockaddr*>(&addr_in6);
    }

    getsockname(sockfd, addr, &addrlen);

    // EFAULT and EINVAL should not happen so they are not checked
    if (errno == EBADF || errno == ENOTSOCK)
	throw std::invalid_argument{"Argument is not a valid socket"};
    if (errno == ENOTSOCK)
	throw std::runtime_error{"Out of resources"};

    if (addr->sa_family != static_cast<int>(ip_version))
	throw std::logic_error{"Socket and address have different protocols"};
}

socket::socket(socket&& other) noexcept
    : handle{other.handle}, ip_version{other.ip_version}
{
    other.handle = SOCKFD_AFTER_MOVE;
}

socket::~socket() noexcept
{
    /*
     * Target of a move operation from this socket is responsible for closing
     * the descriptor and a possible connection
     *
     * Error check is not done here, as the OS generally still closes the file
     * descriptor even if close() fails
     */
    if (handle != SOCKFD_AFTER_MOVE)
	close(handle);
}

auto socket::operator=(socket&& other) noexcept -> socket&
{
    if (this == &other)
	return *this;
    handle = other.handle;
    ip_version = other.ip_version;
    other.handle = SOCKFD_AFTER_MOVE;
    return *this;
}
    
auto socket::to_string() const -> std::string
{
    std::string ip_version_str;
    if (ip_version == ip_protocol::IPv4)
        ip_version_str = "IPv4";
    else
	ip_version_str = "IPv6";       
    return "(SOCK_STREAM, " + ip_version_str + ")";
}
    
} // socket_io
