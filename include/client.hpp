#ifndef SOCKETIO_CLIENT_HPP_
#define SOCKETIO_CLIENT_HPP_

#include <iostream>
#include <string>

#include "address.hpp"
#include "message.hpp"
#include "socket.hpp"

namespace socket_io
{

class client
{
private:
    socket socket;
    address server;
    int id;
public:
    explicit client(address const& server);
    ~client();
    message recv();
    void send(message const& message) const;
    int get_id() const { return id; }
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& stream, client const& client);
}; // client

} // socket_io

#endif // SOCKETIO_CLIENT_HPP_
