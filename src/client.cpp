#include <client.hpp>

#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <typeinfo>
#include <sys/types.h>
#include <utility>

#include <communication.hpp>
#include <connection.hpp>
#include <utils.hpp>

namespace socket_io
{

static inline auto connect(socket const& not_connected, sockaddr* addr,
			   socklen_t addrlen) -> void
{
    if (::connect(not_connected.get_native_handle(), addr, addrlen) == -1)
	throw std::runtime_error{"An error occured when connecting a client: " +
		                 get_errno_as_string()};    
}

client::client(ip_socket_address const& address_of_server)
    : client{address_of_server.type() == typeid(ipv4_socket_address)
	? socket{ip_protocol::IPv4}
        : socket{ip_protocol::IPv6}, address_of_server} {}

client::client(socket&& client,
	       ip_socket_address const& address_of_server)
    : main_socket{std::move(client)}, address_of_server{address_of_server}
{
    bool is_ipv4 = address_of_server.type() == typeid(ipv4_socket_address);
    auto ip_version = main_socket.get_ip_protocol();

    if ((is_ipv4 && ip_version == ip_protocol::IPv6) ||
	(!is_ipv4 && ip_version == ip_protocol::IPv4))
	throw std::logic_error{"Socket and address have different protocols"};
    
    if (is_ipv4)
    {
	auto to_connect = boost::get<ipv4_socket_address>(address_of_server);
	sockaddr_in addr_in = to_connect.get_native_handle();
	socket_io::connect(main_socket, reinterpret_cast<sockaddr*>(&addr_in),
			   sizeof(sockaddr_in));
    }
    else
    {
	auto to_connect = boost::get<ipv6_socket_address>(address_of_server);	
	sockaddr_in6 addr_in6 = to_connect.get_native_handle();
	socket_io::connect(main_socket, reinterpret_cast<sockaddr*>(&addr_in6),
			   sizeof(sockaddr_in6));
    }


    std::string idStr;
    main_socket.make_view() >> idStr;
    id = string_to_integer<int>(idStr);
}
    
client::~client() noexcept
{
    try
    {
	main_socket.make_view() << std::to_string(id);
    }
    catch (...)
    { /* Ignore the exception */ }	
    shutdown(main_socket);
}

auto client::send(std::string const& message) -> void
{
    main_socket.make_view() << "{FROM:" + std::to_string(id) + "} " + message;
}

auto client::receive() -> std::string
{
    std::string buffer;
    main_socket.make_view() >> buffer;
    return buffer;
}
    
} // socket_io
