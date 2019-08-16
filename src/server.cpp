#include <server.hpp>

#include <sys/epoll.h>
#include <limits>
#include <sys/socket.h>
#include <sys/types.h>

#include <communication.hpp>
#include <connection.hpp>
#include <utils.hpp>

namespace socket_io
{

auto server::accept_clients() -> void
{
    for (;;)
    {
	try
	{
	    socket client = accept(main_socket);
	    socket_view view = client.make_view();
	    
	    int const id = connected.get_new_id();
	    view << std::to_string(id);
	    
	    epoll.add(view, EPOLLIN);
	    connected.add(id, std::move(client));
	}
	catch (...)
	{ /* Ignore the exceptions */ }
    }
}

server::server(std::string const& port, ip_protocol const& ip_version)
    : ip_version{ip_version}, main_socket{ip_version},
      address_of_server{ipv4_socket_address{"INADDR_ANY", port}}
{
    if (this->ip_version == ip_protocol::IPv6)
	address_of_server = ipv6_socket_address{"in6addr_any", port};

    bind(main_socket, address_of_server);
    
    if (listen(main_socket.get_native_handle(),
	       std::numeric_limits<int>::max())	== -1)
	throw std::runtime_error{"An error occured on server start: " +
		                 get_errno_as_string()};

    accept_clients_tid = std::thread(&server::accept_clients, this);
    accept_clients_tid.detach();
}

server::~server() noexcept
{
    std::vector<socket> sockets_for_clients = connected.get_clients();
    for (auto& socket : sockets_for_clients)
    {
	socket_view view = socket.make_view();
	
	view << "{- SERVER_EXIT -}";	
	epoll.remove(view);
	shutdown(socket);
    }
    shutdown(main_socket);
    pthread_cancel(accept_clients_tid.native_handle());
}

auto server::receive() -> std::string
{
    auto&& [flags, caused_by] = epoll.get_event();

    std::string message;
    caused_by >> message;
    return message;
}

auto server::send(int const client_id, std::string const& message) -> void
{
    socket client = connected.get_client(client_id);
    client.make_view() << message;
    connected.add(client_id, std::move(client));    
}
    
} // socket_io
