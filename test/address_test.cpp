#include <address.hpp>

#include <iostream>

int main()
{
    using namespace socket_io;
    ipv4_socket_address ipv4_test{"127.0.0.1", "80"};
    ipv6_socket_address ipv6_test{"0:0:0:0:0:0:0:1", "80"};
    ip_socket_address ip_test1 = ipv4_socket_address{"127.0.0.1", "60000"};
    ip_socket_address ip_test2 = ipv6_socket_address{"0:0:0:0:0:0:0:1", "80"};

    std::cout << ipv4_test.to_string() << std::endl;
    std::cout << ipv6_test.to_string() << std::endl;
    std::cout << boost::get<ipv4_socket_address>(ip_test1).to_string()
	      << std::endl;
    std::cout << boost::get<ipv6_socket_address>(ip_test2).to_string()
	      << std::endl;
}
