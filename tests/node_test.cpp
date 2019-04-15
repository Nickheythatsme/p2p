//
// Created by Nick Grout on 2019-04-09.
//

#include "gtest/gtest.h"
// #include "../src/node.h"

#include <iostream>
#include <boost/asio.hpp>


TEST(Node, SmokeTest) {
    // TODO: add node smoke tests
    /*
    node *n = new node;
    delete n;
    n = nullptr;
    */
}

TEST(Node, IOExample) {

    // Provides access to I/O
    boost::asio::io_context io;

    // Set
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

    t.wait();

    std::cout << "timer finished" << std::endl;
}
