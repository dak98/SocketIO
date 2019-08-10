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
     * the descriptor and a possible connection
     *
     * Error check is not done here, as the OS generally still closes the file
     * descriptor even if close() fails
     */
    if (handle != SOCKFD_AFTER_MOVE)
    {
	if (connected_address)
	    shutdown(handle, SHUT_RDWR);
	close(handle);
    }	
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
	throw std::runtime_error{"An error occured when binding a socket: " +
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

static inline auto connect(socket const& client, sockaddr const* addr) -> void
{
    if (::connect(client.get_native_handle(), addr, sizeof(*addr)) == -1)
	throw std::runtime_error{"An error occured when connecting a client: " +
		                 get_errno_as_string()};
}

auto socket::connect(ipv4_socket_address const& address_of_server) -> void
{
    if (connected_address)
	throw std::logic_error{"A client can connect to a server only once"};
    if (ip_version == ip_protocol::IPv6)
	throw std::logic_error{"Cannot connect a IPv4 client to a IPv6 server"};
    sockaddr_in addr = address_of_server.get_native_handle();
    socket_io::connect(*this, reinterpret_cast<sockaddr const*>(&addr));
    connected_address = address_of_server;    
}

auto socket::connect(ipv6_socket_address const& address_of_server) -> void
{
    if (connected_address)
	throw std::logic_error{"A client can connect to a server only once"};
    if (ip_version == ip_protocol::IPv4)
	throw std::logic_error{"Cannot connect a IPv6 client to a IPv4 server"};
    sockaddr_in6 addr = address_of_server.get_native_handle();
    socket_io::connect(*this, reinterpret_cast<sockaddr const*>(&addr));
    connected_address = address_of_server;
}
    
auto socket::to_string() const -> std::string
{
    using boost::get;
    std::string ip_version_str, address, server;
    if (ip_version == ip_protocol::IPv4)
    {
        ip_version_str = "IPv4";
	if (bound_address)
	{
	    auto addr = get<ipv4_socket_address>(bound_address.get());
	    address = addr.to_string();
	}
	if (connected_address)
	{    
	    auto addr = get<ipv4_socket_address>(connected_address.get());
	    server = addr.to_string();
	}
    }
    else
    {
	ip_version_str = "IPv6";
	if (bound_address)
	{
	    auto addr = get<ipv6_socket_address>(bound_address.get());
	    address = addr.to_string();
	}
	if (connected_address)
	{    
	    auto addr = get<ipv6_socket_address>(connected_address.get());
	    server = addr.to_string();
	}
    }

    std::string result;
    if (bound_address)
        result = "(SOCK_STREAM, " + address;
    else
	result = "(SOCK_STREAM, " + ip_version_str;
    if (connected_address)
        result += ", " + server;

    return result + ")";
}
    
} // socket_io
