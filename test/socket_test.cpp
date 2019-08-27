#include <socket.hpp>

#include <gtest/gtest.h>

#include <address.hpp>
#include <connection.hpp>

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

    // is_socket_handle() tests
    EXPECT_FALSE(is_socket_handle(ipv4_native_handle, ip_protocol::IPv6));
    EXPECT_FALSE(is_socket_handle(ipv6_native_handle, ip_protocol::IPv4));

    EXPECT_TRUE(is_socket_handle(ipv4_native_handle, ip_protocol::IPv4));
    EXPECT_TRUE(is_socket_handle(ipv6_native_handle, ip_protocol::IPv6));

    // Protocols mismatch tests
    EXPECT_EQ(ipv4_socket_mock.get_ip_protocol(), ip_protocol::IPv4);
    EXPECT_EQ(ipv6_socket_mock.get_ip_protocol(), ip_protocol::IPv6);

    EXPECT_THROW(socket_io::socket(ipv4_native_handle, ip_protocol::IPv6),
		 std::logic_error);
    EXPECT_THROW(socket_io::socket(ipv6_native_handle, ip_protocol::IPv4),
		 std::logic_error);

    socket_io::socket ipv4_socket_mock2 = std::move(ipv4_socket_mock);
    socket_io::socket ipv6_socket_mock2 = std::move(ipv6_socket_mock);

    EXPECT_FALSE(is_socket_handle(ipv4_socket_mock.get_native_handle(),
				  ip_protocol::IPv4));
    EXPECT_FALSE(is_socket_handle(ipv6_socket_mock.get_native_handle(),
				  ip_protocol::IPv6));

    EXPECT_TRUE(is_socket_handle(ipv4_socket_mock2.get_native_handle(),
				  ip_protocol::IPv4));
    EXPECT_TRUE(is_socket_handle(ipv6_socket_mock2.get_native_handle(),
				  ip_protocol::IPv6));    

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

    // bind() tests
    socket_io::ipv4_socket_address ipv4_address_mock{"127.0.0.1", "0"};
    socket_io::ipv6_socket_address ipv6_address_mock{"::1", "0"};
    EXPECT_THROW(socket_io::bind(ipv4_socket_mock2, ipv6_address_mock),
		 std::logic_error);
    EXPECT_THROW(socket_io::bind(ipv6_socket_mock2, ipv4_address_mock),
		 std::logic_error);

    EXPECT_THROW(socket_io::bind(ipv4_socket_mock, ipv4_address_mock),
		 std::runtime_error);
    EXPECT_THROW(socket_io::bind(ipv6_socket_mock, ipv6_address_mock),
		 std::runtime_error);

    EXPECT_NO_THROW(socket_io::bind(ipv4_socket_mock2, ipv4_address_mock));
    EXPECT_NO_THROW(socket_io::bind(ipv6_socket_mock2, ipv6_address_mock));

    // The socket is already bound to an address
    EXPECT_THROW(socket_io::bind(ipv4_socket_mock2, ipv4_address_mock),
		 std::runtime_error);
    EXPECT_THROW(socket_io::bind(ipv6_socket_mock2, ipv6_address_mock),
		 std::runtime_error);
}
