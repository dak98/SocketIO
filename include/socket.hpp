#ifndef SOCKETIO_SOCKET_HPP_
#define SOCKETIO_SOCKET_HPP_

#include <string>

#include "address.hpp"
#include "protocols.hpp"

namespace socket_io
{

class socket
{
public:
    socket(ip_protocol const& ip_version);

    // Copy operations are deleted as they would splot the objects
    socket(socket const& other) = delete;
    socket(socket&& other) noexcept;

    ~socket();
    
    auto operator=(socket const& other) -> socket& = delete;
    auto operator=(socket&& other) noexcept -> socket&;

    auto get_native_handle() const noexcept -> int { return handle; }
    
    /*
     * @throws std::bad_alloc from std::string constructor
     */
    auto to_string() const -> std::string;
private:
    int handle;
    ip_protocol ip_version;    
};

} // socket_io

#endif // SOCKETIO_SOCKET_HPP_
