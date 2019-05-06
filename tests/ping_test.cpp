//
// Created by Nick Grout on 2019-05-05.
//

#include "../src/listener.hpp"
#include "../src/node.hpp"
#include "gtest/gtest.h"

using namespace p2p;

TEST(Listener, ping_test)
{
    Logger logger("Main");
    Logger::use_console();

    Listener listener;
    int port = 8080;
    Node node("127.0.0.1", port);

    listener.start_listening(port);
    std::this_thread::sleep_for(std::chrono::seconds(1)); // give thread time to start
    node.ping();
    listener.stop_listening();
}
