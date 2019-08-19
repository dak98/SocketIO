#ifndef SOCKETIO_SERVER_HPP_
#define SOCKETIO_SERVER_HPP_

#include <string>
#include <thread>

#include "address.hpp"
#include "epoll.hpp"
#include "protocols.hpp"
#include "registry.hpp"
#include "socket.hpp"

namespace socket_io
{

class server
{
    /*
     * noexcept should be here but it produces compilation error when used with
     * std::thread
     */
    auto accept_clients() -> void;
    std::thread accept_clients_tid;
public:
    /*
     * @throws - std::invalid_argument => argument is not a proper IP address or
     *                                    port
     *         - std::out_of_range => port value out of range
     *         - std::runtime_error => an error occured while binding
     *         - std::runtime_error => an error occured on server start
     *         - std::system_error => the thread to accept clients could not be
     *                                started
     */
    explicit server(std::string const& port, ip_protocol const& ip_version);

    // Copy operations are deleted as main_socket and epoll_handle cannot be copied
    server(server const& other) = delete;
    
    ~server() noexcept;

    auto operator=(server const& other) -> server& = delete;
    /*
     * @throws - std::runtime_error => an error occured while acquiring an event
     *         - std::bad_alloc => from std::string constructor
     */    
    auto receive() -> std::string;
    auto send(int const client_id, std::string const& message) -> void;

    using connected_client = std::pair<int, socket_view>;
    auto get_connected_clients() -> std::vector<connected_client>;
private:
    ip_protocol ip_version;
    socket main_socket;    
    ip_socket_address address_of_server;
    epoll epoll_handle;
    registry_of_clients connected;
}; // server

} // socket_io

#endif // SOCKETIO_SERVER_HPP_
