#include <Address.hpp>
#include <gtest/gtest.h>

#include <exception>

using namespace SocketIO;

TEST(SocketIOTests, AddressTest) {
    EXPECT_THROW(Address(-1), std::domain_error);
    EXPECT_THROW(Address(65536), std::domain_error);
    EXPECT_THROW(Address(1023), std::domain_error);
    EXPECT_NO_THROW(Address(1024));
    EXPECT_NO_THROW(Address(0));
}
