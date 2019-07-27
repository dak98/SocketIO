#ifndef SOCKETIO_ADDRESS_HPP_
#define SOCKETIO_ADDRESS_HPP_

#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

namespace SocketIO {

class Address {
private:
    sockaddr_in _addr;
public:
    static const inline int MIN_PORT = 0;
    static const inline int MAX_PORT = 65535;
    static const inline int MAX_SYSTEM_PORT = 1023;
    /*
     * INADDR_ANY represent all the IP addresses of the machine
     *
     * When port value is 0, OS assigns one of the available ephemeral ports     
     */
    explicit Address(const uint32_t addr = INADDR_ANY, const int port = 0);
    sockaddr_in getAddr() const { return _addr; }
    std::string toString() const {
	return "{family=AF_INET,port=" +
	       std::to_string(_addr.sin_port) +
	    ",addr=" + inet_ntoa(_addr.sin_addr) + "}";
    }
    friend std::ostream& operator<<(std::ostream& stream, const Address& address) {
	return stream << address.toString();
    }
};
    
} // SocketIO

#endif // SOCKETIO_ADDRESS_HPP_
