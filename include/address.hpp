#ifndef SOCKETIO_ADDRESS_HPP_
#define SOCKETIO_ADDRESS_HPP_

#include <cstdint>
#include <netinet/in.h>
#include <string>

namespace socket_io
{

enum class ip_version
{
    IPv4 = AF_INET,
    IPv6 = AF_INET6
};

template<class domain_type>
class socket_address
{
    /*
     * Invariants:
     * - Port should be an integer unsigned in range 0 u [1024, 65535]
     */
    static_assert(std::is_same<domain_type, sockaddr_in>::value ||
		  std::is_same<domain_type, sockaddr_in6>::value,
		  "Only sockaddr_in and sockaddr_in6 are supported");
    using size_type = std::string::size_type;
public:
    socket_address(std::string const& ip_address, uint16_t const port);

    void set_ip_address(std::string const& ip_address);
    void set_port(uint32_t const port) noexcept;

    std::string get_ip_address() const noexcept;
    uint16_t get_port() const noexcept;
    domain_type get_native_handle() const noexcept;

    std::string to_string() const noexcept;
private:
    domain_type handle;
    size_type ip_address_length;
    // is_ipv6 not necessary because of the static_assert
    static bool constexpr is_ipv4 = std::is_same<domain_type, sockaddr_in>::value;
};

template<class domain_type>
inline void socket_address<domain_type>::set_port(uint32_t const port) noexcept
{
    if constexpr(is_ipv4)
        handle.sin_port = htons(port); // To network byte order
    else
	handle.sin6_port = htons(port); // To network byte order
}

template<class domain_type>
inline uint16_t socket_address<domain_type>::get_port() const noexcept
{
    if constexpr(is_ipv4)
        return ntohs(handle.sin_port);
    else
	return ntohs(handle.sin6_port);
}

template<class domain_type>
inline std::string socket_address<domain_type>::to_string() const noexcept
{    
    return "(" + get_ip_address() + ", " + std::to_string(get_port()) + ")";
}

using ipv4_socket_address = socket_address<sockaddr_in>;
using ipv6_socket_address = socket_address<sockaddr_in6>;
    
} // socket_io

#endif // SOCKETIO_ADDRESS_HPP_
