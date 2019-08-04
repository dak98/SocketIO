#ifndef SOCKETIO_ADDRESS_HPP_
#define SOCKETIO_ADDRESS_HPP_

#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

namespace socket_io
{    
    
class inet_address
{
/*
 * Invariants:
 * - Port should be an integer in range 0 u [1024, 65535]
 */    
private:
    sockaddr_in addr;
public:
    static inline int const MIN_PORT = 0;
    static inline int const MAX_PORT = 65535;
    static inline int const MAX_SYSTEM_PORT = 1023;
    /*
     * INADDR_ANY represent all the IP addresses of the machine
     *
     * When port value is 0, OS assigns one of the available ephemeral ports     
     */
    explicit address(uint32_t const s_addr = INADDR_ANY, int const port = 0);
    sockaddr_in get_addr() const { return addr; }
    std::string to_string() const
    {
	return "{family=AF_INET,port=" +
	       std::to_string(addr.sin_port) +
	       ",addr=" + inet_ntoa(addr.sin_addr) + "}";
    }
    friend std::ostream& operator<<(std::ostream& stream, address const& address)
    {
	return stream << address.toString();
    }
};
    
} // socket_io

#endif // SOCKETIO_ADDRESS_HPP_
