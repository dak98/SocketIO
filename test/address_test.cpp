#include <address.hpp>

#include <gtest/gtest.h>

TEST(socket_io_tests, socket_address_tests)
{
    using namespace socket_io;       

    // Correct
    EXPECT_NO_THROW(ipv4_socket_address("127.0.0.1", "80"));
    EXPECT_NO_THROW(ipv4_socket_address("INADDR_ANY", "0"));
    EXPECT_NO_THROW(ipv6_socket_address("::1", "80"));
    EXPECT_NO_THROW(ipv6_socket_address("in6addr_any", "0"));

    // ADDRESS TESTS

    // Address in a wrong format
    std::vector<std::string> non_ipv4_addresses{"ADDR", "in6addr_any", "::1",
	                                        "inaddr_any", ""};
    for (const auto& non_ipv4_address : non_ipv4_addresses)
	EXPECT_THROW(ipv4_socket_address(non_ipv4_address, "0"),
		     std::invalid_argument);
    std::vector<std::string> non_ipv6_addresses{"ADDR", "INADDR_ANY",
	                                        "127.0.0.1", ":1",
	                                        "IN6ADDR_ANY", ""};
    for (const auto& non_ipv6_address : non_ipv6_addresses)
	EXPECT_THROW(ipv6_socket_address(non_ipv6_address, "0"),
		     std::invalid_argument);

    ipv4_socket_address ipv4_address_mock("127.0.0.1", "0");
    ipv6_socket_address ipv6_address_mock("::1", "0");    
    
    // Tests for set_ip_address()
    EXPECT_EQ(ipv4_address_mock.get_ip_address(), "127.0.0.1");
    EXPECT_EQ(ipv6_address_mock.get_ip_address(), "::1");
    for (const auto& non_ipv4_address : non_ipv4_addresses)
	EXPECT_THROW(ipv4_address_mock.set_ip_address(non_ipv4_address),
		     std::invalid_argument);
    for (const auto& non_ipv6_address : non_ipv6_addresses)
	EXPECT_THROW(ipv6_address_mock.set_ip_address(non_ipv6_address),
		     std::invalid_argument);
    // Failed set operations should not cause any changes to the object
    EXPECT_EQ(ipv4_address_mock.get_ip_address(), "127.0.0.1");
    EXPECT_EQ(ipv6_address_mock.get_ip_address(), "::1");    

    EXPECT_NO_THROW(ipv4_address_mock.set_ip_address("127.0.0.1"));
    EXPECT_EQ(ipv4_address_mock.get_ip_address(), "127.0.0.1");
    EXPECT_NO_THROW(ipv6_address_mock.set_ip_address("::1"));
    EXPECT_EQ(ipv6_address_mock.get_ip_address(), "::1");
    
    EXPECT_NO_THROW(ipv4_address_mock.set_ip_address("INADDR_ANY"));
    EXPECT_NE(ipv4_address_mock.get_ip_address(), "127.0.0.1");
    EXPECT_NO_THROW(ipv6_address_mock.set_ip_address("in6addr_any"));
    EXPECT_NE(ipv6_address_mock.get_ip_address(), "::1");

    // PORT TESTS
    
    std::vector<std::string> out_of_range_ports{"-1", "65537", "2147483648"};
    // Port values out of range
    for (const auto& out_of_range_port : out_of_range_ports)
    {
	EXPECT_THROW(ipv4_socket_address("127.0.0.1", out_of_range_port),
		     std::out_of_range);
	EXPECT_THROW(ipv6_socket_address("::1", out_of_range_port),
		     std::out_of_range);
    }
    // Argument is not a valid port value
    std::vector<std::string> invalid_ports{"", "PORT"};
    for (const auto& invalid_port : invalid_ports)
    {
	EXPECT_THROW(ipv4_socket_address("127.0.0.1", invalid_port),
		     std::invalid_argument);
	EXPECT_THROW(ipv6_socket_address("::1", invalid_port),
		     std::invalid_argument);
    }

    // Tests for set_port()
    EXPECT_EQ(ipv4_address_mock.get_port(), "0");
    EXPECT_EQ(ipv6_address_mock.get_port(), "0");
    for (const auto& out_of_range_port : out_of_range_ports)
    {
	EXPECT_THROW(ipv4_address_mock.set_port(out_of_range_port), std::out_of_range);
	EXPECT_THROW(ipv6_address_mock.set_port(out_of_range_port), std::out_of_range);
    }
    
    for (const auto& invalid_port : invalid_ports)
    {
	EXPECT_THROW(ipv4_address_mock.set_port(invalid_port), std::invalid_argument);
	EXPECT_THROW(ipv6_address_mock.set_port(invalid_port), std::invalid_argument);
    }    
    // Failed set operations should not cause any changes to the object
    EXPECT_EQ(ipv4_address_mock.get_port(), "0");
    EXPECT_EQ(ipv6_address_mock.get_port(), "0");
    
    EXPECT_NO_THROW(ipv4_address_mock.set_port("0"));
    EXPECT_EQ(ipv4_address_mock.get_port(), "0");
    EXPECT_NO_THROW(ipv6_address_mock.set_port("0"));
    EXPECT_EQ(ipv6_address_mock.get_port(), "0");

    EXPECT_NO_THROW(ipv4_address_mock.set_port("80"));
    EXPECT_EQ(ipv4_address_mock.get_port(), "80");
    EXPECT_NO_THROW(ipv6_address_mock.set_port("80"));
    EXPECT_EQ(ipv6_address_mock.get_port(), "80");
}
