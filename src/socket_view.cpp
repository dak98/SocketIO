#include <socket_view.hpp>

#include <stdexcept>

#include <connection.hpp>

namespace socket_io
{

socket_view::socket_view(int const handle, ip_protocol const& ip_version)
    : handle{handle}, ip_version{ip_version}
{
    if (!is_socket_handle(handle, ip_version))
	std::invalid_argument{"Not a valid socket handle"};
}

auto socket_view::get_ip_protocol() const -> ip_protocol
{
    if (!is_socket_handle(handle, ip_version))
	std::runtime_error{"Not a valid socket handle"};
    return ip_version;
}

auto socket_view::get_native_handle() const -> int
{
    if (!is_socket_handle(handle, ip_version))
	std::runtime_error{"Not a valid socket handle"};
    return handle;
}

auto socket_view::to_string() const -> std::string
{
    if (!is_socket_handle(handle, ip_version))
	std::runtime_error{"Not a valid socket handle"};

    std::string ip_version_str;
    if (ip_version == ip_protocol::IPv4)
        ip_version_str = "IPv4";
    else
	ip_version_str = "IPv6";       
    return "(SOCK_STREAM, " + ip_version_str + ")";    
}
    
      

} // socket_io    
