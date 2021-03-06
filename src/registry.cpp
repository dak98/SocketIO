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

auto registry_of_clients::add(socket&& new_client) -> int
{
    int const id = get_new_id();

    add(id, std::move(new_client));
    return id;
}

auto registry_of_clients::add(int const id, socket&& client) -> void
{
    if (client_to_socket.find(id) != client_to_socket.end())
	throw std::runtime_error{"Specified id already taken"};
    
    std::lock_guard<std::mutex> lock(add_remove_mutex);
    
    auto result = client_to_socket.emplace(id, std::move(client));

    if (!result.second)
	throw std::invalid_argument{"Failed to add the client to the registry"};
}

auto registry_of_clients::get_client(int const id) -> socket
{
    std::lock_guard<std::mutex> lock(add_remove_mutex);

    socket client = std::move(client_to_socket.at(id));
    client_to_socket.erase(id);
    return client;
}
    
auto registry_of_clients::get_clients() -> std::vector<client_cell>
{
    std::lock_guard<std::mutex> lock(add_remove_mutex);
    
    std::vector<client_cell> sockets;
    for (auto&& [id, sock] : client_to_socket)
	sockets.emplace_back(std::make_pair(id, std::move(sock)));
    client_to_socket.clear();
    return sockets;
}

} // socket_io
