#include <socket.hpp>

#include <gtest/gtest.h>

#include <address.hpp>

TEST(socket_io_tests, socket_tests)
{    
    using socket_io::ip_protocol;

    // Invalid sockfds tests
    std::vector<int> invalid_sockfds{0, -1, 1000};
    for (const auto invalid_sockfd : invalid_sockfds)
    {
	EXPECT_THROW(socket_io::socket(invalid_sockfd, ip_protocol::IPv4),
		     std::invalid_argument);
	EXPECT_THROW(socket_io::socket(invalid_sockfd, ip_protocol::IPv6),
		     std::invalid_argument);
    }

    socket_io::socket ipv4_socket_mock{ip_protocol::IPv4};
    socket_io::socket ipv6_socket_mock{ip_protocol::IPv6};

    int const ipv4_native_handle = ipv4_socket_mock.get_native_handle();
    int const ipv6_native_handle = ipv6_socket_mock.get_native_handle();
    
    // Protocols mismatch tests
    EXPECT_EQ(ipv4_socket_mock.get_ip_protocol(), ip_protocol::IPv4);
    EXPECT_EQ(ipv6_socket_mock.get_ip_protocol(), ip_protocol::IPv6);

    EXPECT_THROW(socket_io::socket(ipv4_native_handle, ip_protocol::IPv6),
		 std::logic_error);
    EXPECT_THROW(socket_io::socket(ipv6_native_handle, ip_protocol::IPv4),
		 std::logic_error);
    
    socket_io::socket ipv4_socket_mock2 = std::move(ipv4_socket_mock);
    socket_io::socket ipv6_socket_mock2 = std::move(ipv6_socket_mock);

    EXPECT_EQ(ipv4_socket_mock2.get_native_handle(), ipv4_native_handle);
    EXPECT_EQ(ipv4_socket_mock2.get_ip_protocol(), ip_protocol::IPv4);
    EXPECT_EQ(ipv6_socket_mock2.get_native_handle(), ipv6_native_handle);
    EXPECT_EQ(ipv6_socket_mock2.get_ip_protocol(), ip_protocol::IPv6);

    // socket_view tests
    auto const ipv4_socket_view = ipv4_socket_mock2.make_view();
    auto const ipv6_socket_view = ipv6_socket_mock2.make_view();

    EXPECT_EQ(ipv4_socket_view.get_native_handle(), ipv4_native_handle);
    EXPECT_EQ(ipv6_socket_view.get_native_handle(), ipv6_native_handle);

    EXPECT_EQ(ipv4_socket_view.get_ip_protocol(), ip_protocol::IPv4);
    EXPECT_EQ(ipv6_socket_view.get_ip_protocol(), ip_protocol::IPv6);    
}
