#include <epoll.hpp>

#include <gtest/gtest.h>
#include <socket.hpp>

TEST(socket_io_tests, epoll_test)
{
    using namespace socket_io;

    epoll epoll_mock;

    socket_io::socket ipv4_socket_mock{ip_protocol::IPv4};
    socket_io::socket ipv6_socket_mock{ip_protocol::IPv6};

    epoll_mock.add(ipv4_socket_mock.make_view(), 0);
    epoll_mock.add(ipv6_socket_mock.make_view(), 0);

    EXPECT_NO_THROW(epoll_mock.remove(ipv4_socket_mock.make_view()));
    EXPECT_NO_THROW(epoll_mock.remove(ipv6_socket_mock.make_view()));

    // Nothing should happend if socket is not being monitored
    EXPECT_NO_THROW(epoll_mock.remove(ipv4_socket_mock.make_view()));
    EXPECT_NO_THROW(epoll_mock.remove(ipv6_socket_mock.make_view()));    
    
    int epoll_handle = epoll_mock.get_native_handle();

    epoll epoll_mock2 = std::move(epoll_mock);
    EXPECT_EQ(epoll_mock2.get_native_handle(), epoll_handle);       
}

