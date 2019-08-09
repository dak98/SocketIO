#include <address.hpp>

#include <iostream>

int main()
{
    using namespace socket_io;
    ipv4_socket_address ipv4_test{"127.0.0.1", "80"};
    ipv6_socket_address ipv6_test{"0:0:0:0:0:0:0:1", "80"};

    std::cout << ipv4_test.to_string() << std::endl;
    std::cout << ipv6_test.to_string() << std::endl;
}
