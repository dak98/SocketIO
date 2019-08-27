#include <client.hpp>

#include <gtest/gtest.h>

TEST(socket_io_tests, client_tests)
{
    using socket_io::ip_protocol;
    using socket_io::ipv4_socket_address;
    using socket_io::ipv6_socket_address;
    using socket_io::client;
    
    socket_io::socket ipv4_socket_moc{ip_protocol::IPv4};
    socket_io::socket ipv6_socket_moc{ip_protocol::IPv6};
    ipv4_socket_address ipv4_address_mock{"127.0.0.1", "0"};
    ipv6_socket_address ipv6_address_mock{"::1", "0"};    
    
    EXPECT_THROW(client(std::move(ipv4_socket_moc), ipv6_address_mock),
		 std::logic_error);
    EXPECT_THROW(client(std::move(ipv6_socket_moc), ipv4_address_mock),
		 std::logic_error);

    // The ownership of the socket has been lost
    EXPECT_THROW(client(std::move(ipv4_socket_moc), ipv4_address_mock),
		 std::runtime_error);
    EXPECT_THROW(client(std::move(ipv6_socket_moc), ipv6_address_mock),
		 std::runtime_error);

    /*
     * Correct behaviour is not checked as it requires a running server which is
     * out of scope of a unit test
     */ 
}
