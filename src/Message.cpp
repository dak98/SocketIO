#include <Message.hpp>

namespace SocketIO {
    
inline std::string Message::toString() const {
    return "[sockfd=" +	std::to_string(getSockfd()) +
	",type=" + std::to_string(getMessageType()) +
	",message=" + _message + "]";
}

std::ostream& operator<<(std::ostream& stream, const Message& message) {
    return stream << message.toString();
}

} // SocketIO
