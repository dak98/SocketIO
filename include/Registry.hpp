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
    explicit Registry(int const baseId = 100)
	: baseId{baseId} {}

    int add(const int sockfd);

    int getSockfdById(int const id) const { return idToSockfd.at(id); }
    int getIdBySockfd(int const sockfd) const { return sockfdToId.at(sockfd); }

    void removeBySockfd(int const sockfd);
    void removeById(int const id);

    std::vector<int> getIds() const;
    std::vector<int> getSockfds() const;
}; // Registry
    
} // SocketIO

#endif // SOCKETIO_REGISTRY_HPP_
