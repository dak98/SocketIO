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

/*
 * The address of the client which connected is not returned as the server does
 * not need it for communication
 *
 * @throws - std::runtime_error => an error occured while accepting a client 
 */
auto accept(socket const& server) -> socket;

auto shutdown(socket const& connected) noexcept -> void;

auto is_socket_handle(int const handle, ip_protocol const& ip_version) noexcept
    -> bool;
    
} // socket_io

#endif // SOCKETIO_CONNECTION_HPP_
