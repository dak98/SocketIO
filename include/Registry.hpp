#ifndef SOCKETIO_REGISTRY_HPP_
#define SOCKETIO_REGISTRY_HPP_

#include <iostream>

#include <limits>
#include <unordered_map>
#include <vector>

namespace SocketIO
{

// It is NOT thread-safe.
class Registry
{
private:
    using ClientSocketMap = std::unordered_map<int, int>;
    ClientSocketMap idToSockfd;
    ClientSocketMap sockfdToId;

    const int baseId;
public:
    
    
    explicit Registry(const int baseId = 100)
	: baseId{baseId} {}

    int add(const int sockfd);

    int getSockfdById(const int id) const { return idToSockfd.at(id); }
    int getIdBySockfd(const int sockfd) const { return sockfdToId.at(sockfd); }

    void removeBySockfd(const int sockfd);
    void removeById(const int id);

    std::vector<int> getIds() const;
    std::vector<int> getSockfds() const;
}; // Registry
    
} // SocketIO

#endif // SOCKETIO_REGISTRY_HPP_
