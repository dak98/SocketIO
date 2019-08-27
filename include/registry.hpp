#ifndef SOCKETIO_REGISTRY_HPP_
#define SOCKETIO_REGISTRY_HPP_

#include <boost/functional/hash.hpp>

#include <functional>
#include <mutex>
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
 */
class registry_of_clients
{
    using client_id = int;
    auto get_new_id() const -> int;
public:
    explicit registry_of_clients(int const base_id = 100)
	: base_id{base_id} {}

    /*
     * NOTE: Only r-value reference version for add() methods is provided as the
     * socket class is move-only
     */
    
    /*
     * @return Id of the newly added client in the registry
     * @throws - std::runtime_error => no free ID available
     *         - std::invalid_argument => failed to add a client
     *         - std::system_error => failed to lock a mutex
     */
    auto add(socket&& new_client) -> int;
    /*
     * @throws - std::invalid_argument => failed to add a client
     *         - std::system_error => failed to lock a mutex
     *         - std::runtime_error => specified id already taken
     */
    auto add(int const id, socket&& client) -> void;

    /*
     * @brief Removes the socket from the registry and then returns it
     * @throws - std::out_of_range => element with specified key does not exist
     *         - std::system_error => failed to lock a mutex
     */
    auto get_client(int const id) -> socket;

    using client_cell = std::pair<client_id, socket>;
    /*
     * @brief Removes all the sockets from the registry and then returns them
     * @throws - std::bad_alloc => could not allocate the vector
     *         - std::system_error => failed to lock a mutex
     */
    auto get_clients() -> std::vector<client_cell>;
private:
    int const base_id;
    std::unordered_map<client_id, socket> client_to_socket;

    mutable std::mutex get_new_id_mutex;
    std::mutex add_remove_mutex;
}; // registry_of_clients
    
} // socket_io

#endif // SOCKETIO_REGISTRY_HPP_
