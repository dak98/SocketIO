#include <socket.hpp>

#include <iostream>

int main()
{
    using socket_io::ip_protocol;
    socket_io::socket socket1_test{ip_protocol::IPv4};
    socket_io::socket socket2_test{ip_protocol::IPv6};

    std::cout << socket1_test.to_string() << std::endl;
    std::cout << socket2_test.to_string() << std::endl;    
}
