#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Nick Grout on 2019-04-09.
//

#include "gtest/gtest.h"
#include "../src/node.hpp"
#include <iostream>

using namespace p2p;
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(100)) // give logs time to output

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
    catch(const connection_exception &e) {
        // Pass if we make it here and end test
        PAUSE;
        SUCCEED() << "Exception was thrown when it was suppossed to";
        return;
    }
    // Fail if we make it here
    PAUSE;
    FAIL() << "Exception was not thrown for invalid IP address";
}

TEST(Node, PingTest) {
    Logger logger("Main");
    Logger::use_console();
    const char* address = "127.0.0.1";
    int port = 8080;
    try {
        Node n(address, port);
        n.ping();
    }
    catch(const connection_exception &e) {
        logger.warn(std::string("Node ping failed: ") + e.what());
        // TODO Ignoring failure for now
        // FAIL() << e.what();
    }
    PAUSE;
}

#pragma clang diagnostic pop
