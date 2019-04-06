
#include "gtest/gtest.h"
#include <iostream>
#include "../src/udp_server.hpp"
#include "../src/udp_client.hpp"

using std::cout;
using std::endl;

TEST(UDPTest, server)
{
    udp_server::run();
}

TEST(UDPTest, client)
{
    udp_client::run();
}

