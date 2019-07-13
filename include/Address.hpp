#ifndef SOCKETIO_ADDRESS_HPP_
#define SOCKETIO_ADDRESS_HPP_

#include <iostream>
#include <netinet/in.h>
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
     * When port value is 0, OS assigns one of the available ephemeral ports
     */
    explicit Address(const int port = 0);
    sockaddr_in getAddr() const { return _addr; }
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& stream, const Address& address);
};
    
} // SocketIO

#endif // SOCKETIO_ADDRESS_HPP_
