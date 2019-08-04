#ifndef SOCKETIO_SERVER_HPP_
#define SOCKETIO_SERVER_HPP_

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "Address.hpp"
#include "Epoll.hpp"
#include "Message.hpp"
#include "Registry.hpp"
#include "Socket.hpp"

namespace SocketIO
{

class Server
{
private:
    Address address;
    Socket socket;
    Epoll epoll;
    Registry registry; // Registry of the clients' ids
    std::thread acceptClientsTID;
    void acceptClients();
public:
    explicit Server(int const port = 0);
    ~Server();
    void launch();
    Message recv();
    void send(Message const& message) const;
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& stream, Server const& server);
};

} // SocketIO

#endif // SOCKETIO_SERVER_HPP_
