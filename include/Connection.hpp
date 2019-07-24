#ifndef SOCKETIO_CONNECTION_HPP_
#define SOCKETIO_CONNECTION_HPP_

#include <Message.hpp> 

namespace SocketIO {

Message recv(const int fd);
void send(const Message& message);
    
} // SocketIO

#endif