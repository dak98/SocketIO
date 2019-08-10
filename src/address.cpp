#include <address.hpp>

#include <cstdint>
#include <arpa/inet.h>
#include <stdexcept>

#include <utils.hpp>

namespace socket_io
{
    
template<class T>
socket_address<T>::socket_address(std::string const& ip_address,
				  std::string const& port)
    : handle{} // Set the structure to zero
{
    set_ip_address(ip_address);
    set_port(port);
    if constexpr(is_ipv4)
	handle.sin_family = AF_INET;
    else
	handle.sin6_family = AF_INET6;
}
    
template<class T>    
auto socket_address<T>::set_ip_address(std::string const& ip_address) -> void
{
    int error_code;
    if constexpr(is_ipv4)
        error_code = inet_pton(AF_INET, ip_address.c_str(),
			     static_cast<void*>(&handle.sin_addr));
    else
	error_code = inet_pton(AF_INET6, ip_address.c_str(),
			     static_cast<void*>(&handle.sin6_addr));
    /*
     * Error checking for EAFNOSUPPORT no necessary as AF_INET and AF_INET6 are
     * valid address families
    */
    if (error_code == 0)
	throw std::invalid_argument{ip_address + " is not a valid IP address"};
    ip_address_length = ip_address.length();
}

template<class T>
auto socket_address<T>::set_port(std::string const& port) -> void
{
    long const port_as_integer = string_to_integer<uint16_t>(port);
    if constexpr(is_ipv4)
        handle.sin_port = htons(port_as_integer); // To network byte order
    else
	handle.sin6_port = htons(port_as_integer); // To network byte order
}    

template<class T>
auto socket_address<T>::get_ip_address() const -> std::string
{
    size_type const buffer_length = ip_address_length + 1;
    char buffer[buffer_length];
    /* 
     * Error checking is no necessary as:
     * - AF_INET or AF_INET6 is a valid address family (no EAFNOSUPPORT)
     * - ip_address_length is a proper length of the address (no ENOSPC)
     */
    if constexpr(is_ipv4)
        inet_ntop(AF_INET, &handle.sin_addr, buffer, buffer_length);
    else
	inet_ntop(AF_INET6, &handle.sin6_addr, buffer, buffer_length);
    return buffer;
}

template<class T>
auto socket_address<T>::get_port() const -> std::string
{
    long port_as_integer;
    if constexpr(is_ipv4)
	port_as_integer = ntohs(handle.sin_port);
    else
	port_as_integer = ntohs(handle.sin6_port);
    return std::to_string(port_as_integer);
}
    
template<class T>
auto socket_address<T>::to_string() const -> std::string
{    
    std::string address_family;
    if constexpr(is_ipv4)
        address_family = "IPv4";
    else
	address_family = "IPv6";
    return "(" + address_family +   ", " +
	         get_ip_address() + ", " +
	         get_port() + ")";
}
    
template class socket_address<sockaddr_in>;
template class socket_address<sockaddr_in6>;
    
} // socket_io
