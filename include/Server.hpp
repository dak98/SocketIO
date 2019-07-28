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

namespace SocketIO
{

class Server
{
private:
    Address address;
    Socket socket;
    Epoll epoll;
    std::vector<Socket> clients;
    std::thread acceptClientsTID;
    void acceptClients();
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

#endif // SOCKETIO_SERVER_HPP_
