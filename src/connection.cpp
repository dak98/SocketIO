#include <connection.hpp>

#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <typeinfo>
#include <sys/types.h>

#include <utils.hpp>

namespace socket_io
{
    
auto bind(socket const& unbound, ip_socket_address const& to_bind) -> void
{
    bool is_ipv4 = to_bind.type() == typeid(ipv4_socket_address);
    auto ip_version = unbound.get_ip_protocol();

    if ((is_ipv4 && ip_version == ip_protocol::IPv6) ||
	(!is_ipv4 && ip_version == ip_protocol::IPv4))
	throw std::logic_error{"Socket and address have different protocols"};
    
    sockaddr* addr;
    socklen_t addrlen;
    if (is_ipv4)
    {
	auto tmp_to_bind = boost::get<ipv4_socket_address>(to_bind);
	
        addrlen = sizeof(sockaddr_in);
	sockaddr_in addr_in = tmp_to_bind.get_native_handle();
	addr = reinterpret_cast<sockaddr*>(&addr_in);	
    }
    else
    {
	auto tmp_to_bind = boost::get<ipv6_socket_address>(to_bind);
	
        addrlen = sizeof(sockaddr_in6);
	sockaddr_in6 addr_in6 = tmp_to_bind.get_native_handle();
	addr = reinterpret_cast<sockaddr*>(&addr_in6);
    }

    if (::bind(unbound.get_native_handle(), addr, addrlen) == -1)
	throw std::runtime_error{"An error occured when binding a socket: " +
		                 get_errno_as_string()};    
}
    
} // socket_io
