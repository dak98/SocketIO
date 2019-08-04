#ifndef SOCKETIO_ADDRESS_HPP_
#define SOCKETIO_ADDRESS_HPP_

#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

namespace SocketIO
{

class Address
{
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
    explicit Address(uint32_t const s_addr = INADDR_ANY, int const port = 0);
    sockaddr_in getAddr() const { return addr; }
    std::string toString() const
    {
	return "{family=AF_INET,port=" +
	       std::to_string(addr.sin_port) +
	       ",addr=" + inet_ntoa(addr.sin_addr) + "}";
    }
    friend std::ostream& operator<<(std::ostream& stream, Address const& address)
    {
	return stream << address.toString();
    }
};
    
} // SocketIO

#endif // SOCKETIO_ADDRESS_HPP_
