#include <address.hpp>

#include <arpa/inet.h>
#include <stdexcept>

namespace socket_io
{
    
template<class domain_type>
socket_address<domain_type>::socket_address(std::string const& ip_address,
					    uint16_t const port)
    : ip_address_length{ip_address.length()}
{
    int error_code;
    if constexpr(is_ipv4)
    {
        error_code = inet_pton(AF_INET, ip_address.c_str(),
			       static_cast<void*>(&handle.sin_addr));
	handle.sin_family = AF_INET;
	handle.sin_port = htons(port); // To network byte order
    }
    else
    {	
	handle = sockaddr_in6{}; // Set the structure to zero
	error_code = inet_pton(AF_INET6, ip_address.c_str(),
			       static_cast<void*>(&handle.sin6_addr));
	handle.sin6_family = AF_INET6;
	handle.sin6_port = htons(port); // To network byte order
    }
    if (error_code == 0)
	throw std::invalid_argument{ip_address + " is not in a valid format"};
}

template class socket_address<sockaddr_in>;
template class socket_address<sockaddr_in6>;
    
template<class domain_type>    
void socket_address<domain_type>::set_ip_address(std::string const& ip_address)
{
    int error_code;
    if constexpr(is_ipv4)
        error_code = inet_pton(AF_INET, ip_address.c_str(),
			     static_cast<void*>(&handle.sin_addr));
    else
	error_code = inet_pton(AF_INET6, ip_address.c_str(),
			     static_cast<void*>(&handle.sin6_addr));
    if(error_code == 0)
	throw std::invalid_argument{ip_address + " is not in a valid format"};
    ip_address_length = ip_address.length();
}

template<class domain_type>    
std::string socket_address<domain_type>::get_ip_address() const noexcept
{
    size_type const buffer_length = ip_address_length + 1;
    char* buffer = new char[buffer_length];
    /* Error checking is no necessary as:
     * - AF_INET or AF_INET6 is a valid address family (no EAFNOSUPPORT)
     * - ip_address_length is a proper length of the address (no ENOSPC)
     */    
    if constexpr(is_ipv4)
        inet_ntop(AF_INET, &handle.sin_addr, buffer, buffer_length);
    else
	inet_ntop(AF_INET6, &handle.sin6_addr, buffer, buffer_length);
    return buffer;
}
    
} // socket_io
