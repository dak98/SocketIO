#include <registry.hpp>

#include <gtest/gtest.h>

TEST(socket_io_tests, registry_tests)
{
    using socket_io::ip_protocol;
    
    socket_io::registry_of_clients registry;

    EXPECT_THROW(registry.get_client(100), std::out_of_range);

    socket_io::socket client_mock1{ip_protocol::IPv4};
    socket_io::socket client_mock2{ip_protocol::IPv6};

    int const sockfd1 = client_mock1.get_native_handle();
    int const sockfd2 = client_mock2.get_native_handle();

    int const id1 = registry.add(std::move(client_mock1));
    int const id2 = registry.add(std::move(client_mock2));

    auto clients = registry.get_clients();
    EXPECT_EQ(clients.size(), 2);

    registry.add(clients[0].first, std::move(clients[0].second));
    registry.add(clients[1].first, std::move(clients[1].second));

    EXPECT_NO_THROW(client_mock1 = registry.get_client(id1));
    EXPECT_NO_THROW(client_mock2 = registry.get_client(id2));

    EXPECT_EQ(client_mock1.get_native_handle(), sockfd1);
    EXPECT_EQ(client_mock2.get_native_handle(), sockfd2);

    EXPECT_EQ(client_mock1.get_ip_protocol(), ip_protocol::IPv4);
    EXPECT_EQ(client_mock2.get_ip_protocol(), ip_protocol::IPv6);

    clients = registry.get_clients();
    EXPECT_EQ(clients.size(), 0);
}
