#include <registry.hpp>

#include <limits>
#include <stdexcept>

namespace socket_io
{

auto registry_of_clients::get_new_id() const -> int
{
    int id = base_id;
    bool found_id = false;
    for (; id < std::numeric_limits<int>::max() && !found_id; id++)
	if (client_to_socket.find(id) == client_to_socket.end())
	    found_id = true;
    if (!found_id)
	throw std::runtime_error{"No free ID available"};
    return id;    
}

auto registry_of_clients::add(int const id, socket&& socket) -> void
{
    auto result = client_to_socket.emplace(id, std::move(socket));

    if (result.second)
	throw std::invalid_argument{"Failed to add the client to the registry"};
}

auto registry_of_clients::get_client(int const id) -> socket
try
{
    client_to_socket.erase(id);
    return std::move(client_to_socket.at(id));
}
catch (...)
{ /* Ignore the exceptions */ }
    
auto registry_of_clients::get_clients() -> std::vector<socket>
{
    std::vector<socket> sockets;
    for (auto&& [id, sock] : client_to_socket)
	sockets.emplace_back(std::move(sock));
    return sockets;
}
    
} // socket_io
