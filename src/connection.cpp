#include <communication.hpp>

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>

#include <utils.hpp>

namespace socket_io
{

static inline auto bind(socket const& unbound, sockaddr const* addr) -> void
{
    if (::bind(unbound.get_native_handle(), addr, sizeof(*addr)) == -1)
	throw std::runtime_error{"An error occured when binding a socket: " +
		                 get_errno_as_string()};
}
    
auto bind(socket const& unbound, ip_socket_address const& to_bind) -> void
{
    if (unbound.get_ip_version() == ip_protocol::IPv6)
        throw std::logic_error{"Cannot bind an IPv4 address to a IPv6 socket"};
    sockaddr_in addr = to_bind.get_native_handle();
    socket_io::bind(*this, reinterpret_cast<sockaddr const*>(&addr));
}

auto bind(socket const& unbound, ipv6_socket_address const& to_bind) -> void
{
    if (unbound.get_ip_version() == ip_protocol::IPv4)
	throw std::logic_error{"Cannot bind an IPv6 address to a IPv4 socket"};
    sockaddr_in6 addr = to_bind.get_native_handle();
    socket_io::bind(*this, reinterpret_cast<sockaddr const*>(&addr));
}

static inline auto connect(socket const& client, sockaddr const* addr) -> void
{
    if (::connect(client.get_native_handle(), addr, sizeof(*addr)) == -1)
	throw std::runtime_error{"An error occured when connecting a client: " +
		                 get_errno_as_string()};
}
    
auto connect(socket const& client, ipv4_socket_address const& server) -> void
{
    if (unbound.get_ip_version() == ip_protocol::IPv6)
	throw std::logic_error{"Cannot connect a IPv4 client to a IPv6 server"};
    sockaddr_in addr = address_of_server.get_native_handle();
    socket_io::connect(*this, reinterpret_cast<sockaddr const*>(&addr));
}

auto connect(socket const& client, ipv6_socket_address const& server) -> void
{
    if (unbound.get_ip_version() == ip_protocol::IPv4)
	throw std::logic_error{"Cannot connect a IPv6 client to a IPv4 server"};
    sockaddr_in6 addr = address_of_server.get_native_handle();
    socket_io::connect(*this, reinterpret_cast<sockaddr const*>(&addr));
}
    
} // socket_io
