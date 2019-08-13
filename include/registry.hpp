#ifndef SOCKETIO_REGISTRY_HPP_
#define SOCKETIO_REGISTRY_HPP_

#include <boost/functional/hash.hpp>

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "socket.hpp"
    
namespace socket_io
{

/*
 * Holds a bimap between:
 * - ID of the client given by the server,
 * - a socket assigned to the server for communication with the client.
 *
 * It is NOT thread-safe
 */
class registry_of_clients
{
    using client_id = int;
    using socket_for_client = std::pair<int, ip_protocol>;
    using hash = boost::hash<socket_for_client>;
public:
    explicit registry_of_clients(int const base_id = 100)
	: base_id{base_id} {}

    /*
     * @returns Id of the new entry
     * @throws - std::runtime_error => no free ID available
     */
    auto add(socket const& for_client) -> int;

    /*
     * @throws - std::out_of_range => element with specified key does not exist
     */
    auto get_socket(int const id) const -> socket;
    auto get_id(socket const& for_client) const -> int;

    // If the client is not registered, nothing happends    
    auto remove(socket const& for_client) noexcept -> void;
    auto remove(int const id) noexcept -> void;

    /*
     * @throws - std::bad_alloc => could not allocate the vector
     */
    auto get_ids() const -> std::vector<client_id>;
    auto get_sockets() const -> std::vector<socket>;
private:
    int const base_id;
    std::unordered_map<client_id, socket_for_client> client_to_socket;
    std::unordered_map<socket_for_client, client_id, hash> socket_to_client;
}; // registry_of_clients
    
} // socket_io

#endif // SOCKETIO_REGISTRY_HPP_
