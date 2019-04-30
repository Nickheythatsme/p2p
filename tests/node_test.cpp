//
// Created by Nick Grout on 2019-04-09.
//

#include "gtest/gtest.h"
#include "../src/node.hpp"
#include <iostream>


TEST(Node, SmokeTest) {
    Node *n = new Node("8.8.8.8", 0);
    delete n;
    n = nullptr;
}

TEST(Node, InvalidAddress) {
    try {
        Node n("ffff.8.8.8", 0);
    }
    catch(const node_exception &e) {
        // Pass if we make it here and end test
        SUCCEED() << "Exception was thrown when it was suppossed to";
        return;
    }
    // Fail if we make it here
    FAIL() << "Exception was not thrown when it was suppossed to";
}

TEST(Node, PingTest) {
    const char* address ="68.179.215.230";
    try {
        Node n(address, 4343);
        n.ping();
    }
    catch(const node_exception &e) {
        std::cerr << e.what() << std::endl;
        // TODO remove once ping is enabled
        // FAIL() << e.what();
    }
}
