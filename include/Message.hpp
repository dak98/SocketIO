#ifndef SOCKETIO_MESSAGE_HPP_
#define SOCKETIO_MESSAGE_HPP_

#include <iostream>
#include <string>

#include "Socket.hpp"

namespace SocketIO
{

enum MessageType
{
    UNIT_EXIT = 0,
    INIT_MSG  = 1,
    REG_MSG = 2,
};

class Message
{
private:
    int id;
    MessageType type;
    std::string message;
public:
    Message() = default;
    Message(const int id, MessageType type, const std::string& message)
	: id{id}, type{type}, message{message} {}
    int getId() const { return id; }
    MessageType getMessageType() const { return type; }
    std::string getMessage() const { return message; }
    std::string toString() const
    {
	return "{id=" + std::to_string(id) +
	       ",type=" + std::to_string(getMessageType()) +
	       ",message=" + message + "}";
    }
    friend std::ostream& operator<<(std::ostream& stream, const Message& message)
    {
	return stream << message.toString();
    }
};
    
} // SocketIO

#endif // SOCKETIO_MESSAGE_HPP_
