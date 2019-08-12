#ifndef SOCKETIO_REGISTRY_HPP_
#define SOCKETIO_REGISTRY_HPP_

#include <unordered_map>
#include <vector>

namespace socket_io
{

// It is NOT thread-safe.
class registry_of_clients
{
    using client_id = int;
    using socket_id = int;
public:
    explicit registry_of_clients(int const base_id = 100)
	: base_id{base_id} {}

    /*
     * @returns Id of the new entry
     * @throws - std::runtime_error => no free ID available
     */
    auto add(int const sockfd) -> int;

    /*
     * @throws - std::out_of_range => element with specified key does not exist
     */
    auto get_sockfd(int const id) const -> int
    { return client_to_socket.at(id); }
    auto get_id(int const sockfd) const -> int
    { return socket_to_client.at(sockfd); }

    auto remove_by_sockfd(int const sockfd) noexcept -> void;
    auto remove_by_id(int const id) noexcept -> void;

    /*
     * @throws - std::bad_alloc => could not allocate the vector
     */
    auto get_ids() const -> std::vector<client_id>;
    auto get_sockfds() const -> std::vector<socket_id>;
private:
    int const base_id;    
    std::unordered_map<client_id, socket_id> client_to_socket;
    std::unordered_map<socket_id, client_id> socket_to_client;
}; // registry_of_clients
    
} // socket_io

#endif // SOCKETIO_REGISTRY_HPP_
