#ifndef SOCKETIO_CLIENT_HPP_
#define SOCKETIO_CLIENT_HPP_

#include <Address.hpp>
#include <iostream>
#include <Message.hpp>
#include <Socket.hpp>
#include <string>

namespace SocketIO
{

class Client
{
private:
    Socket socket;
    Address server;
public:
    Client() = delete;
    explicit Client(const Address& addressOfServer);
    ~Client();
    Message recv();
    void send(const Message& message) const;
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& stream, const Client& client);
}; // Client

} // SocketIO

#endif // SOCKETIO_CLIENT_HPP_
