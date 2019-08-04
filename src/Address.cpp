#include <Address.hpp>

#include <endian.h>
#include <stdexcept>	
#include <string>

namespace SocketIO
{

Address::Address(uint32_t const s_addr, int const port)
{
    if (port < MIN_PORT || port > MAX_PORT)
	throw std::domain_error("Port value should be in range [" +
				std::to_string(MIN_PORT) + "; " +
				std::to_string(MAX_PORT) + "]");
    if (port <= MAX_SYSTEM_PORT && port != MIN_PORT)
	throw std::domain_error("Ports in range [" +
				std::to_string(MIN_PORT) + "; " +
				std::to_string(MAX_SYSTEM_PORT) + "]" +
				" are system ports and should not be used.");
    addr.sin_family = AF_INET;
    addr.sin_port = htobe16(static_cast<const in_port_t>(port));
    addr.sin_addr.s_addr = s_addr;
}
    
} // SocketIO
