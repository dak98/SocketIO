#ifndef SOCKETIO_MESSAGE_HPP_
#define SOCKETIO_MESSAGE_HPP_

#include <iostream>
#include <string>

#include "socket.hpp"

namespace socket_io
{

enum message_type
{
    UNIT_EXIT = 0,
    INIT_MSG  = 1,
    REG_MSG = 2,
};

class message
{
private:
    int id;
    message_type type;
    std::string content;
public:
    message() = default;
    message(int const id, message_type const type, std::string const& content)
	: id{id}, type{type}, content{content} {}
    int get_id() const { return id; }
    message_type get_type() const { return type; }
    std::string get_content() const { return content; }
    std::string to_string() const
    {
	return "{id=" + std::to_string(id) +
	       ",type=" + std::to_string(get_type()) +
	       ",content=" + content + "}";
    }
    friend std::ostream& operator<<(std::ostream& stream, message const& message)
    {
	return stream << message.to_string();
    }
};
    
} // socket_io

#endif // SOCKETIO_MESSAGE_HPP_
