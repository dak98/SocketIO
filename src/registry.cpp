#include <registry.hpp>

#include <limits>
#include <stdexcept>

namespace socket_io
{

auto registry_of_clients::add(socket const& socket) -> int
{
    int id = base_id;
    bool found_id = false;
    for (; id < std::numeric_limits<int>::max() && !found_id; id++)
	if (client_to_socket.find(id) == client_to_socket.end())
	{
	    socket_for_client const sock{socket.get_native_handle(),
		                         socket.get_ip_protocol()};
	    client_to_socket.emplace(id, sock);
	    socket_to_client.emplace(sock, id);
	    found_id = true;
	}

    if (!found_id)
	throw std::runtime_error{"No free ID available"};
    return id;
}

auto registry_of_clients::get_socket(int const id) const -> socket
{
    socket_for_client const sock = client_to_socket.at(id);
    return socket{sock.first, sock.second};
}

auto registry_of_clients::get_id(socket const& socket) const -> int
{
    socket_for_client const sock{socket.get_native_handle(),
	                         socket.get_ip_protocol()};
    return socket_to_client.at(sock);        
}

auto registry_of_clients::remove(socket const& socket) noexcept -> void
{
    try
    {
	int const id = get_id(socket);
	socket_for_client const sock = client_to_socket.at(id);
	client_to_socket.erase(id);
	socket_to_client.erase(sock);
    }
    catch (...)
    { /* Ignore the exceptions */ }
}

auto registry_of_clients::remove(int const id) noexcept -> void
{
    try
    {	
	socket_for_client const sock = client_to_socket.at(id);
	client_to_socket.erase(id);
	socket_to_client.erase(sock);
    }
    catch (...)
    { /* Ignore the exceptions */ }    
}

auto registry_of_clients::get_ids() const -> std::vector<int>
{
    std::vector<int> ids(client_to_socket.size());
    int index = 0;
    for (auto const& [id, sock] : client_to_socket)
	ids[index++] = id;
    return ids;
}

auto registry_of_clients::get_sockets() const -> std::vector<socket>
{
    std::vector<socket> sockets;
    for (auto const& [sock, id] : socket_to_client)
	sockets.emplace_back(socket{sock.first, sock.second});
    return sockets;
}    
    
} // socket_io
