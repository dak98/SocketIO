#include <registry.hpp>

#include <limits>
#include <stdexcept>

namespace socket_io
{

auto registry_of_clients::add(int const sockfd) -> int
{
    int id = base_id;
    bool found_id = false;
    for (; id < std::numeric_limits<int>::max() && !found_id; id++)
	if (client_to_socket.find(id) == client_to_socket.end())
	{
	    client_to_socket.emplace(id, sockfd);
	    socket_to_client.emplace(sockfd, id);
	    found_id = true;
	}

    if (!found_id)
	throw std::runtime_error{"No free ID available"};
    return id;
}

auto registry_of_clients::remove_by_sockfd(int const sockfd) noexcept -> void
{
    client_to_socket.erase(get_id(sockfd));
    socket_to_client.erase(sockfd);
}

auto registry_of_clients::remove_by_id(int const id) noexcept -> void
{
    client_to_socket.erase(id);
    socket_to_client.erase(get_sockfd(id));
}

auto registry_of_clients::get_ids() const -> std::vector<int>
{
    std::vector<int> ids(client_to_socket.size());
    int index = 0;
    for (auto const& [id, sockfd] : client_to_socket)
	ids[index++] = id;
    return ids;
}

auto registry_of_clients::get_sockfds() const -> std::vector<int>
{
    std::vector<int> sockfds(socket_to_client.size());
    int index = 0;
    for (auto const& [sockfd, id] : socket_to_client)
	sockfds[index++] = sockfd;
    return sockfds;
}    
    
} // socket_io
