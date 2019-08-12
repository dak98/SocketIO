#ifndef SOCKETIO_CLIENT_HPP_
#define SOCKETIO_CLIENT_HPP_

#include <string>

#include "address.hpp"
#include "socket.hpp"

namespace socket_io
{

class client
{
public:
    /*
     * @throws - std::runtime_error => an error occured while creating a socket
     *         - std::runtime_error => an error occured while connecting to the
     *                                 server
     */
    explicit client(ip_socket_address const& address_of_server);
    /*
     *  Constructor with l-value reference not provided as the socket class
     *  cannot be copied
     */
    /*
     * @throws - std::logic_error => socket and address have different protocols
     */
    client(socket&& main_socket, ip_socket_address const& address_of_server);

    // Copy operations are deleted as sockets cannot be copied
    client(client const& other) = delete;
    
    ~client() noexcept;

    auto operator=(socket const& other) -> socket& = delete;

    auto send(std::string const& message) -> void;
    auto receive() -> std::string;

    auto get_address_of_server() const noexcept -> ip_socket_address
    { return address_of_server; }
private:
    socket main_socket;
    ip_socket_address address_of_server;
}; // client

} // socket_io

#endif // SOCKETIO_CLIENT_HPP_
