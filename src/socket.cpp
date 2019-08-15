#include <socket.hpp>

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <connection.hpp>
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
    : handle{sockfd}, ip_version{ip_version}
{
    if (!is_socket_handle(sockfd, ip_version))
	std::invalid_argument{"Not a valid socket handle"};
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
