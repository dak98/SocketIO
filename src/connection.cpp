#include <connection.hpp>

#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <typeinfo>
#include <sys/types.h>

#include <utils.hpp>

namespace socket_io
{

static inline auto bind_wrapper(socket const& unbound, sockaddr* addr,
				socklen_t addrlen) -> void
{
    if (::bind(unbound.get_native_handle(), addr, addrlen) == -1)
	throw std::runtime_error{"An error occured when binding a socket: " +
		                 get_errno_as_string()};    
}
    
auto bind(socket const& unbound, ip_socket_address const& to_bind) -> void
{
    bool is_ipv4 = to_bind.type() == typeid(ipv4_socket_address);
    auto ip_version = unbound.get_ip_protocol();

    if ((is_ipv4 && ip_version == ip_protocol::IPv6) ||
	(!is_ipv4 && ip_version == ip_protocol::IPv4))
	throw std::logic_error{"Socket and address have different protocols"};
    
    if (is_ipv4)
    {
	auto tmp_to_bind = boost::get<ipv4_socket_address>(to_bind);	
	sockaddr_in addr_in = tmp_to_bind.get_native_handle();
	socket_io::bind_wrapper(unbound, reinterpret_cast<sockaddr*>(&addr_in),
				sizeof(sockaddr_in));
	
    }
    else
    {
	auto tmp_to_bind = boost::get<ipv6_socket_address>(to_bind);	
	sockaddr_in6 addr_in6 = tmp_to_bind.get_native_handle();
	socket_io::bind_wrapper(unbound, reinterpret_cast<sockaddr*>(&addr_in6),
				sizeof(sockaddr_in6));
    }
}

auto accept(socket const& server) -> socket
{
    int sockfd = ::accept(server.get_native_handle(), nullptr, nullptr);
    if (sockfd == -1)
	throw std::runtime_error{"An error occured while accepting a client: " +
				 get_errno_as_string()};
    return socket{sockfd, server.get_ip_protocol()};
}

auto shutdown(socket const& connected) noexcept -> void
{
    shutdown(connected.get_native_handle(), SHUT_RDWR);
}

auto is_socket_handle(int const handle, ip_protocol const& ip_version) noexcept
    -> bool
{
    sockaddr addr;
    socklen_t addrlen = sizeof(sockaddr);

    getsockname(handle, &addr, &addrlen);

    if (addrlen == sizeof(sockaddr_in) && ip_version == ip_protocol::IPv6)
	return false;
    if (addrlen == sizeof(sockaddr_in6) && ip_version == ip_protocol::IPv4)
	return false;	

    // EFAULT and EINVAL should not happen so they are not checked
    if (errno == EBADF || errno == ENOTSOCK)
	return false;
    if (errno == ENOTSOCK)
	return false;

    return true;
}

    
} // socket_io
