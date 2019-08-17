#include <registry.hpp>

#include <limits>
#include <stdexcept>

namespace socket_io
{

auto registry_of_clients::get_new_id() const -> int
{
    std::lock_guard<std::mutex> lock(get_new_id_mutex);
    
    int id = base_id;
    bool found_id = false;
    while (id < std::numeric_limits<int>::max() && !found_id)
	if (client_to_socket.find(id) == client_to_socket.end())
	    found_id = true;
	else
	    id++;
    if (!found_id)
	throw std::runtime_error{"No free ID available"};
    return id;    
}

auto registry_of_clients::add(int const id, socket&& socket) -> void
{
    std::lock_guard<std::mutex> lock(add_remove_mutex);
    
    auto result = client_to_socket.emplace(id, std::move(socket));

    if (!result.second)
	throw std::invalid_argument{"Failed to add the client to the registry"};
}

auto registry_of_clients::get_client(int const id) -> socket
{
    std::lock_guard<std::mutex> lock(add_remove_mutex);
    
    client_to_socket.erase(id);
    return std::move(client_to_socket.at(id));
}
    
auto registry_of_clients::get_clients() -> std::vector<socket>
{
    std::lock_guard<std::mutex> lock(add_remove_mutex);
    
    std::vector<socket> sockets;
    for (auto&& [id, sock] : client_to_socket)
	sockets.emplace_back(std::move(sock));
    return sockets;
}
    
} // socket_io
