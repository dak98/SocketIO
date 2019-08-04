#ifndef SOCKETIO_REGISTRY_HPP_
#define SOCKETIO_REGISTRY_HPP_

#include <iostream>

#include <limits>
#include <unordered_map>
#include <vector>

namespace socket_io
{

// It is NOT thread-safe.
class registry
{
private:
    using client_socket_map = std::unordered_map<int, int>;
    client_socket_map id_to_sockfd;
    client_socket_map sockfd_to_id;

    const int base_id;
public:        
    explicit registry(int const baseId = 100)
	: base_id{base_id} {}

    /*
     * @return Id of the new entry. 
     */
    int add(const int sockfd);

    int get_sockfd(int const id) const { return id_to_sockfd.at(id); }
    int get_id(int const sockfd) const { return sockfd_to_id.at(sockfd); }

    // TODO: Refactor.
    void removeBySockfd(int const sockfd);
    void removeById(int const id);

    std::vector<int> get_ids() const;
    std::vector<int> get_sockfds() const;
}; // registry
    
} // socket_io

#endif // SOCKETIO_REGISTRY_HPP_
