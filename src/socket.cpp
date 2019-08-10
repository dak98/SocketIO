#include <socket.hpp>

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>

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

socket::socket(socket&& other) noexcept
    : handle{other.handle}, ip_version{other.ip_version}
{
    other.handle = SOCKFD_AFTER_MOVE;
}

socket::~socket() noexcept
{
    /*
     * Target of a move operation from this socket is responsible for closing
     * the descriptor
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

static inline auto bind(socket const& unbound, sockaddr const* addr) -> void
{
    if (::bind(unbound.get_native_handle(), addr, sizeof(*addr)) == -1)
	throw std::runtime_error{"An error occured while binding a socket: " +
		                 get_errno_as_string()};
}

auto socket::bind(ipv4_socket_address const& to_bind) -> void
{
    if (bound_address)
	throw std::logic_error{"A socket can be bound to an address only once"};
    if (ip_version == ip_protocol::IPv6)
	throw std::logic_error{"Cannot bind an IPv4 address to a IPv6 socket"};
    sockaddr_in addr = to_bind.get_native_handle();
    socket_io::bind(*this, reinterpret_cast<sockaddr const*>(&addr));
    bound_address = to_bind;
}

auto socket::bind(ipv6_socket_address const& to_bind) -> void
{
    if (bound_address)
	throw std::logic_error{"A socket can be bound to an address only once"};
    if (ip_version == ip_protocol::IPv4)
	throw std::logic_error{"Cannot bind an IPv6 address to a IPv4 socket"};
    sockaddr_in6 addr = to_bind.get_native_handle();
    socket_io::bind(*this, reinterpret_cast<sockaddr const*>(&addr));
    bound_address = to_bind;
}
    
auto socket::to_string() const -> std::string
{
    using boost::get;
    std::string ip_version_str, address;
    if (ip_version == ip_protocol::IPv4)
    {
        ip_version_str = "IPv4";
	if (bound_address)
	    address = get<ipv4_socket_address>(bound_address.get()).to_string();
    }
    else
    {
	ip_version_str = "IPv6";
	if (bound_address)
	    address = get<ipv6_socket_address>(bound_address.get()).to_string();
    }
    if (bound_address)
        return "(SOCK_STREAM, " + address + ")";
    return "(SOCK_STREAM, " + ip_version_str + ")";
}
    
} // socket_io
