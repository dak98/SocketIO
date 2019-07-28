#ifndef SOCKETIO_MESSAGE_HPP_
#define SOCKETIO_MESSAGE_HPP_

#include <iostream>
#include <Socket.hpp>
#include <string>

namespace SocketIO
{

enum MessageType
{
    UNIT_EXIT = 0
};

class Message
{
private:
    Socket socket;
    MessageType type;
    std::string message;
public:
    Message() = default;
    Message(const int sockfd, MessageType type, const std::string& message)
	: socket{sockfd}, type{type}, message{message} {}
    int getSockfd() const { return socket.getSockfd(); }
    MessageType getMessageType() const { return type; }    
    std::string getMessage() const { return message; }
    std::string toString() const
    {
	return "{sockfd=" + std::to_string(getSockfd()) +
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
