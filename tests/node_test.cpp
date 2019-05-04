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
  Node *n = new Node("8.8.8.8", 0);
  delete n;
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
  const char* address ="127.0.0.1";
  Logger logger("Main");
  try {
    Node n(address, 20);
    n.ping();
  }
  catch(const node_exception &e) {
    logger.warn(std::string("Node ping failed: ") + e.what());
    // TODO remove once ping is enabled
    // FAIL() << e.what();
  }
}

#pragma clang diagnostic pop