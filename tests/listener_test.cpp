//
// Created by Nick Grout on 2019-09-03.
//

#include "gtest/gtest.h"
#include "record.h"
#include "util/hash.hpp"
#include "util/hash_table.h"
#include "networking/listener.h"
#include "networking/socket.h"
#include <vector>

using std::cout;
using std::endl;
using namespace p2p::networking;

TEST(Listener, smoke) {
    auto f = init_addrinfo("www.example.com", nullptr);
    Listener l;
}

