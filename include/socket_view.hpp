#ifndef SOCKETIO_SOCKET_VIEW_HPP_
#define SOCKETIO_SOCKET_VIEW_HPP_

#include <string>

#include "protocols.hpp"

namespace socket_io
{

class socket;

class socket_view
{
public:
    socket_view() = delete;

    /*
     * @throws - std::runtime_error => not a valid socket handle
     */
    auto get_ip_protocol() const -> ip_protocol;
    auto get_native_handle() const -> int;
    
    /*
     * @throws - std::bad_alloc => from std::string constructor
     *         - std::runtime_error => not a valid socket handle
     */
    auto to_string() const -> std::string;
private:
    socket_view(int const handle, ip_protocol const& ip_version);
    
    int handle;
    ip_protocol ip_version;

    friend class socket;
};

} // socket_io

#endif // SOCKETIO_SOCKET_VIEW_HPP_
