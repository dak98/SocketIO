#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <string>

#include "protocols.hpp"

namespace socket_io
{

class socket
{
public:
    /*
     * @throws - std::runtime_error => an error occured while creating a socket
     */
    explicit socket(ip_protocol const& ip_version);
    /*
     * @throws - std::logic_error => socket and address have different protocols
     *         - std::invalid_argument => Argument is not a valid socket
     *         - std::runtime_error => out of resources
     */
    socket(int const sockfd, ip_protocol const& ip_version);

    // Copy operations are deleted as they would splot the objects
    socket(socket const& other) = delete;
    socket(socket&& other) noexcept;

    ~socket() noexcept;
    
    auto operator=(socket const& other) -> socket& = delete;
    auto operator=(socket&& other) noexcept -> socket&;

    auto get_ip_protocol() const -> ip_protocol { return ip_version; }

    auto get_native_handle() const noexcept -> int { return handle; }
    
    /*
     * @throws - std::bad_alloc => from std::string constructor
     */
    auto to_string() const -> std::string;
private:
    int handle;
    ip_protocol ip_version;
};

} // socket_io

#endif // SOCKETIO_SOCKET_HPP_
