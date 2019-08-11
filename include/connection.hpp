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
    
/*
 * @throws - std::logic_error => ip protocols mismatch
 *         - std::runtime_error => an error occured while connecting to the
 *                                 server
 */
auto connect(socket const& client, ipv4_socket_address const& server) -> void;
auto connect(socket const& client, ipv6_socket_address const& server) -> void;    
    
} // socket_io

#endif // SOCKETIO_CONNECTION_HPP_
