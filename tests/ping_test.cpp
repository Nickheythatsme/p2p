//
// Created by Nick Grout on 2019-05-05.
//

#include "../src/listener.hpp"
#include "../src/node.hpp"
#include "gtest/gtest.h"

using namespace p2p;
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(100)) // give logs time to output

TEST(Listener, ping_test)
{
    Logger logger("Main");
    Logger::use_console();

    Listener listener;
    int port = 8080;
    Node node("127.0.0.1", "8080");

    listener.start_listening(port);
    PAUSE; // Give the listener thread time to start
    node.ping();
    listener.stop_listening();
    PAUSE;
}
