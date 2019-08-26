#ifndef SOCKETIO_ADDRESS_HPP_
#define SOCKETIO_ADDRESS_HPP_

#include <boost/variant.hpp>

#include <netinet/in.h>
#include <string>

namespace socket_io
{

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
    /*
     * @throws - std::invalid_argument => argument is not a proper IP address or
     *                                    port
     *         - std::out_of_range => port value out of range
     */
    socket_address(std::string const& ip_address, std::string const& port);

    /*
     * Both INADDR_ANY and in6addr_any constants are supported
     *
     * Exception safety: Strong
     * @throws - std::invalid_argument => argument is not a proper IP address
     */
    auto set_ip_address(std::string const& ip_address) -> void;
    /* 
     * Exception safety: Strong
     * @throws - std::invalid_argument => argument is not a proper port
     *         - std::out_of_range => port value out of range
     */    
    auto set_port(std::string const& port) -> void;

    /*
     * @throws - std::bad_alloc => from std::string constructor
     */
    auto get_ip_address() const -> std::string;
    auto get_port() const -> std::string;
    
    auto get_native_handle() const noexcept -> domain_type { return handle; }

    /*
     * @throws - std::bad_alloc => from std::string constructor
     */
    auto to_string() const -> std::string;
private:
    domain_type handle;
    size_type ip_address_length;
    // is_ipv6 not necessary because of the static_assert
    static bool constexpr is_ipv4 =
	std::is_same<domain_type, sockaddr_in>::value;
};

using ipv4_socket_address = socket_address<sockaddr_in>;
using ipv6_socket_address = socket_address<sockaddr_in6>;
using ip_socket_address =
    boost::variant<ipv4_socket_address, ipv6_socket_address>;
    
} // socket_io

#endif // SOCKETIO_ADDRESS_HPP_
