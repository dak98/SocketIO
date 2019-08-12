#ifndef SOCKETIO_CONNECTION_HPP_
#define SOCKETIO_CONNECTION_HPP_

#include "address.hpp"
#include "socket.hpp"

namespace socket_io
{

/*
 * @throws - std::logic_error => socket and address have different protocols
 *         - std::runtime_error => an error occured while binding
 */
auto bind(socket const& unbound, ip_socket_address const& to_bind) -> void;
    
} // socket_io

#endif // SOCKETIO_CONNECTION_HPP_
