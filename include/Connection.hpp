#ifndef SOCKETIO_CONNECTION_HPP_
#define SOCKETIO_CONNECTION_HPP_

#include "Message.hpp"

namespace SocketIO
{

Message recv(int const fd);
void send(int const fd, Message const& message);
    
} // SocketIO

#endif // SOCKETIO_CONNECTION_HPP_
