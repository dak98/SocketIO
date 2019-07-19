#ifndef SOCKETIO_SERVER_HPP_
#define SOCKETIO_SERVER_HPP_

#include <Address.hpp>
#include <Client.hpp>
#include <Epoll.hpp>
#include <iostream>
#include <Message.hpp>
#include <Socket.hpp>
#include <string>
#include <thread>
#include <vector>

namespace SocketIO {

class Server {
private:
    Address _address;
    Socket _socket;
    Epoll _epoll;
    std::vector<Socket> _clients;
    std::thread _acceptClientsTID;
    void _acceptClients();
public:
    explicit Server(const int port = 0);
    ~Server();
    void launch();
    Message recv();
    void send(const Message& message) const;
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& stream, const Server& server);
};

} // SocketIO

#endif
