#include <client.hpp>

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>

#include <communication.hpp>
#include <utils.hpp>

namespace socket_io
{

static inline auto connect(socket const& client, sockaddr const* addr) -> void
{
    if (::connect(client.get_native_handle(), addr, sizeof(*addr)) == -1)
	throw std::runtime_error{"An error occured when connecting a client: " +
		                 get_errno_as_string()};
}
    
client::client(ipv4_socket_address const& address_of_server)
    : address_of_server{address_of_server}, main_socket{ip_protocol::IPv4}
{
    sockaddr_in addr = address_of_server.get_native_handle();
    socket_io::connect(main_socket, reinterpret_cast<sockaddr const*>(&addr));
}

client::client(ipv6_socket_address const& address_of_server)
    : address_of_server{address_of_server}, main_socket{ip_protocol::IPv6}
{
    sockaddr_in6 addr = address_of_server.get_native_handle();
    socket_io::connect(main_socket, reinterpret_cast<sockaddr const*>(&addr));
}
    
client::~client() noexcept
{
    shutdown(main_socket.get_native_handle(), SHUT_RDWR);
}

auto client::send(std::string const& message) -> void
{
    main_socket << message;
}

auto client::receive() -> std::string
{
    std::string buffer;
    main_socket >> buffer;
    return buffer;
}
    
} // socket_io
