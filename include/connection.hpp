#ifndef SOCKETIO_CONNECTION_HPP_
#define SOCKETIO_CONNECTION_HPP_

#include "message.hpp"

namespace socket_io
{

message recv(int const fd);
void send(int const fd, message const& message);
    
} // socket_io

#endif // SOCKETIO_CONNECTION_HPP_
