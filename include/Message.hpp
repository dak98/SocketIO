#ifndef SOCKETIO_MESSAGE_HPP_
#define SOCKETIO_MESSAGE_HPP_

#include <iostream>
#include <Socket.hpp>
#include <string>

namespace SocketIO {

enum MessageType {
    UNIT_EXIT = 0
};

class Message {
private:
    Socket _socket;
    MessageType _type;
    std::string _message;
public:
    Message() = default;
    Message(const int sockfd, MessageType type, const std::string& message)
	: _socket{sockfd},_type{type}, _message{message} {}
    int getSockfd() const { return _socket.getSockfd(); }
    MessageType getMessageType() const { return _type; }    
    std::string getMessage() const { return _message; }
    std::string toString() const {
	return "{sockfd=" +	std::to_string(getSockfd()) +
	    ",type=" + std::to_string(getMessageType()) +
	    ",message=" + _message + "}";
    }
    friend std::ostream& operator<<(std::ostream& stream, const Message& message) {
	return stream << message.toString();
    }
};
    
} // SocketIO

#endif // SOCKETIO_MESSAGE_HPP_
