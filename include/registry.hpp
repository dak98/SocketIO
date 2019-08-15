#ifndef SOCKETIO_REGISTRY_HPP_
#define SOCKETIO_REGISTRY_HPP_

#include <boost/functional/hash.hpp>

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "socket.hpp"
#include "socket_view.hpp"
    
namespace socket_io
{

/*
 * Holds a map between:
 * - ID of the client given by the server,
 * - a socket assigned to the server for communication with the client.
 *
 * It is NOT thread-safe
 */
class registry_of_clients
{
    using client_id = int;
public:
    explicit registry_of_clients(int const base_id = 100)
	: base_id{base_id} {}

    /*
     * @throws - std::runtime_error => no free ID available
     */    
    auto get_new_id() const -> int;
    

    auto add(int const id, socket&& for_client) -> void;

    /*
     * @brief Removes the socket from the registry and then returns it
     * @throws - std::out_of_range => element with specified key does not exist
     */
    auto get_client(int const id) -> socket;

    /*
     * @brief Removes all the sockets from the registry and then returns them
     * @throws - std::bad_alloc => could not allocate the vector
     */
    auto get_clients() -> std::vector<socket>;
private:
    int const base_id;
    std::unordered_map<client_id, socket> client_to_socket;
}; // registry_of_clients
    
} // socket_io

#endif // SOCKETIO_REGISTRY_HPP_
