#ifndef SOCKETIO_SERVER_HPP_
#define SOCKETIO_SERVER_HPP_

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "address.hpp"
#include "epoll.hpp"
#include "message.hpp"
#include "registry.hpp"
#include "socket.hpp"

namespace socket_io
{

class server
{
private:
    address address;
    socket main_socket;
    epoll epoll;
    registry connected_clients;
    
    void accept_clients();
    std::thread accept_clients_tid;
public:
    explicit server(int const port = 0);
    ~server();
    void launch();
    message recv();
    void send(message const& message) const;
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& stream, server const& server);
}; // server

} // socket_io

#endif // SOCKETIO_SERVER_HPP_
