#ifndef SOCKETIO_COMMUNICATION_HPP_
#define SOCKETIO_COMMUNICATION_HPP_

#include <string>

#include "socket.hpp"
#include "socket_view.hpp"

namespace socket_io
{

/*
 * @throws - std::runtime_error => an error occured while sending the data
 */    
auto operator<<(socket_view& dst, std::string const& to_send) -> socket_view&;

/*
 * @throws - std::bad_alloc => from std::string constructor
 *         - std::runtime_error => an error occured while sending the data
 *         - std::system_error => other side has closed the connection
 */
auto operator>>(socket_view& src, std::string& buffer) -> socket_view&;
    
} // socket_io

#endif
