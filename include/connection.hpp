#ifndef SOCKETIO_CONNECTION_HPP_
#define SOCKETIO_CONNECTION_HPP_

#include "address.hpp"
#include "socket.hpp"

namespace socket_io
{

/*
 * @throws - std::logic_error => ip protocols mismatch
 *         - std::runtime_error => an error occured while binding
 */
auto bind(socket const& unbound, ipv4_socket_address const& to_bind) -> void;
auto bind(socket const& unbound, ipv6_socket_address const& to_bind) -> void;
    
} // socket_io

#endif // SOCKETIO_CONNECTION_HPP_
