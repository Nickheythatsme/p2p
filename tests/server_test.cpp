//
// Created by Nick Grout on 2019-04-09.
//

#include "gtest/gtest.h"
#include "../src/server.hpp"

#include <iostream>

using namespace p2p;

TEST(Server, Smoke) {
    server s;
    s.listen(8080);
}
