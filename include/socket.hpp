#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <boost/optional.hpp>

#include <string>

#include "address.hpp"
#include "protocols.hpp"

namespace socket_io
{

class socket
{
public:
    /*
     * @throws - std::runtime_error => an error occured while creating a socket
     */
    socket(ip_protocol const& ip_version);    

    // Copy operations are deleted as they would splot the objects
    socket(socket const& other) = delete;
    socket(socket&& other) noexcept;

    ~socket() noexcept;
    
    auto operator=(socket const& other) -> socket& = delete;
    auto operator=(socket&& other) noexcept -> socket&;

    auto get_native_handle() const noexcept -> int { return handle; }

    /*
     * @throws - std::logic_error => socket already bound
     *         - std::logic_error => ip protocols mismatch
     *         - std::runtime_error => an error occured while binding
     */
    auto bind(ipv4_socket_address const& to_bind) -> void;
    auto bind(ipv6_socket_address const& to_bind) -> void;

    /*
     * @throws - std::logic_error => client already connected
     *         - std::logic_error => ip protocols mismatch
     *         - std::runtime_error => an error occured while connecting
     */    
    auto connect(ipv4_socket_address const& address_of_server) -> void;
    auto connect(ipv6_socket_address const& address_of_server) -> void;    

    /*
     * @returns The address to which the socket was bound
     */
    auto get_bound_address() const noexcept
	-> boost::optional<ip_socket_address> { return bound_address; }
    /*
     * @returns The address to which the client was connected
     */    
    auto get_address_of_server() const noexcept
	-> boost::optional<ip_socket_address> { return connected_address; }
    
    /*
     * @throws - std::bad_alloc => from std::string constructor
     */
    auto to_string() const -> std::string;
private:
    int handle;
    ip_protocol ip_version;
    boost::optional<ip_socket_address> bound_address;
    boost::optional<ip_socket_address> connected_address;    
};

} // socket_io

#endif // SOCKETIO_SOCKET_HPP_
