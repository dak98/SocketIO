#include <communication.hpp>

#include <cstdio>
#include <arpa/inet.h>
#include <limits>
#include <sys/socket.h>
#include <stdexcept>
#include <system_error>
#include <sys/types.h>

#include <utils.hpp>

namespace socket_io
{

static inline auto send(int sockfd, void const* buf, size_t len) -> void
{
    int error_code = ::send(sockfd, buf, len, MSG_NOSIGNAL);
    if (error_code == -1 && errno == EPIPE)
	throw std::system_error{std::make_error_code(std::errc::broken_pipe)};
    else if (error_code == -1)    
	throw std::runtime_error{"An error occured while sending the data: " +
		                 get_errno_as_string()};
}

auto operator<<(socket_view& dst, std::string const& to_send) -> socket_view&
{
    // Prepare data to send
    std::string::size_type const length = to_send.length();
    if (length > std::numeric_limits<uint32_t>::max())
	throw std::invalid_argument{"Input message is too long"};
    uint32_t with_byte_order = htonl(static_cast<uint32_t>(length));
    
    char buffer[length + 1];
    if (snprintf(buffer, length + 1, "%s", to_send.c_str()) < 0)
	throw std::invalid_argument{"Input message is wrong: " +
		                    get_errno_as_string()};
    
    // Send the data
    send(dst.get_native_handle(), static_cast<void*>(&with_byte_order),
	 sizeof(uint32_t));
    send(dst.get_native_handle(), static_cast<void*>(&buffer), length);

    return dst;
}

auto operator<<(socket_view&& dst, std::string const& to_send) -> socket_view&
{
    return dst << to_send;
}

static inline auto recv(int sockfd, void* buf, size_t len) -> int
{
    ssize_t error_code = ::recv(sockfd, buf, len, 0);
    if (error_code == 0)
	throw std::system_error{std::make_error_code(std::errc::broken_pipe)};
    if (error_code == -1)
	throw std::runtime_error{"An error occured while reading the data: " +
		                 get_errno_as_string()};
    return error_code;
}

// TODO: Handle peer disconnect
auto operator>>(socket_view& src, std::string& buffer) -> socket_view&
{
    uint32_t with_byte_order;
    recv(src.get_native_handle(), static_cast<void*>(&with_byte_order),
	 sizeof(uint32_t));

    // No error should occur as the data length sent was of size_type
    std::string::size_type const length = ntohl(with_byte_order);

    char buf[length + 1];
    recv(src.get_native_handle(), static_cast<void*>(&buf), length);
    buf[length] = '\0';

    buffer = std::string{buf};
    return src;
}

auto operator>>(socket_view&& src, std::string& buffer) -> socket_view&
{
    return src >> buffer;
}

} // socket_io
