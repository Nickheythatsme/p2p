#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Nick Grout on 2019-04-09.
//

#include "gtest/gtest.h"
#include "../src/node.hpp"
#include <iostream>

using namespace p2p;

TEST(Node, SmokeTest) {
    Node *n = new Node("0.0.0.0", 0);
    delete n;
}

TEST(Node, InvalidAddress) {
    Logger logger("Main");
    try {
        Node n("ffff.8.8.8", 0);
        n.ping();
    }
    catch(const node_exception &e) {
        // Pass if we make it here and end test
        SUCCEED() << "Exception was thrown when it was suppossed to";
        return;
    }
    // Fail if we make it here
    FAIL() << "Exception was not thrown for invalid IP address";
}

TEST(Node, PingTest) {
    Logger logger("Main");
    const char* address ="127.0.0.1";
    try {
        Node n(address, 22);
        n.ping();
    }
    catch(const node_exception &e) {
        logger.warn(std::string("Node ping failed: ") + e.what());
        FAIL() << e.what();
    }
}

#pragma clang diagnostic pop
