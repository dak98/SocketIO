#include <iostream>

#include <address.hpp>
#include <socket.hpp>

int main()
{
    using socket_io::ip_protocol;
    socket_io::socket socket1_test{ip_protocol::IPv4};
    socket_io::socket socket2_test{ip_protocol::IPv6};

    std::cout << socket1_test.to_string() << std::endl;
    std::cout << socket2_test.to_string() << std::endl;

    socket_io::ipv4_socket_address ipv4_test{"127.0.0.1", "8000"};
    socket_io::ipv6_socket_address ipv6_test{"0:0:0:0:0:0:0:1", "80"};

    socket1_test.bind(ipv4_test);
    //    socket2_test.bind(ipv6_test);

    std::cout << socket1_test.to_string() << std::endl;
    std::cout << socket2_test.to_string() << std::endl;
}
