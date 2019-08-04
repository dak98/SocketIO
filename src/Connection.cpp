#include <Connection.hpp>

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <stdexcept>
#include <sys/types.h>
#include <vector>

#include <iterator>
#include <string.h>

namespace SocketIO {

static void checkErrors(ssize_t const length)
{    
    if (length == -1)
	throw std::runtime_error{"An error occured while reading a message: " +
		static_cast<std::string>(std::strerror(errno))};
}

static inline bool hasDisconnected(ssize_t const length)
{
    return (length == 0);
}

Message recv(int const fd)
{
    ssize_t length;

    uint32_t id;
    length = ::recv(fd, reinterpret_cast<void*>(&id),
		    sizeof(uint32_t), MSG_NOSIGNAL);
    checkErrors(length);
    if (hasDisconnected(length))
	return {0, UNIT_EXIT, ""};
    
    uint32_t type;
    length = ::recv(fd, reinterpret_cast<void*>(&type),
		    sizeof(uint32_t), MSG_NOSIGNAL);
    checkErrors(length);
    if (hasDisconnected(length))
	return {0, UNIT_EXIT, ""};
    
    uint32_t len;
    length = ::recv(fd, reinterpret_cast<void*>(&len),
		    sizeof(uint32_t), MSG_NOSIGNAL);
    checkErrors(length);    
    if (hasDisconnected(length))
	return {0, UNIT_EXIT, ""};

    char buff[be32toh(len)];
    length = ::recv(fd, reinterpret_cast<void*>(&buff),
		    be32toh(len), MSG_NOSIGNAL);

    checkErrors(length);
    if (hasDisconnected(length))
	return {0, UNIT_EXIT, ""};
    
    return Message(be32toh(id), static_cast<MessageType>(be32toh(type)), buff);
}

void send(int const fd, Message const& message)
{
    ssize_t length;

    uint32_t const id = htobe32(message.getId());
    length = ::send(fd, reinterpret_cast<const void*>(&id),
		    sizeof(uint32_t), MSG_NOSIGNAL);
    checkErrors(length);

    uint32_t const type = htobe32(message.getMessageType());
    length = ::send(fd, reinterpret_cast<const void*>(&type),
		    sizeof(uint32_t), MSG_NOSIGNAL);
    checkErrors(length);

    std::string const msg = message.getMessage();
    char msgStr[msg.length()];
    for (size_t i = 0; i < msg.length(); i++)
	msgStr[i] = msg[i];
    
    uint32_t const len = htobe32(msg.length());
    length = ::send(fd, reinterpret_cast<const void*>(&len),
			    sizeof(uint32_t), MSG_NOSIGNAL);    
    checkErrors(length);

    length = ::send(fd, reinterpret_cast<const void*>(&msgStr),
		    msg.length(), MSG_NOSIGNAL);
    checkErrors(length);
}
    
} // SocketIO
