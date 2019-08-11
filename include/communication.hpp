#ifndef SOCKETIO_COMMUNICATION_HPP_
#define SOCKETIO_COMMUNICATION_HPP_

#include <string>

#include "socket.hpp"

namespace socket_io
{

auto operator<<(socket& dst, std::string const& to_send) -> socket&;

/*
 * @throws - std::bad_alloc => from std::string constructor
 */
auto operator>>(socket& src, std::string& buffer) -> socket&;
    
} // socket_io

#endif
