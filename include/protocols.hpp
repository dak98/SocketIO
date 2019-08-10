#ifndef SOCKETIO_PROTOCOLS_HPP_
#define SOCKETIO_PROTOCOLS_HPP_

#include <netinet/in.h>

namespace socket_io
{

enum class ip_protocol
{
    IPv4 = AF_INET,
    IPv6 = AF_INET6
};

} // socket_io
    
#endif // SOCKETIO_PROTOCOLS_HPP_
