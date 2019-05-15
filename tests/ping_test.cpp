//
// Created by Nick Grout on 2019-05-05.
//

#include "../src/listener.hpp"
#include "../src/node.hpp"
#include "gtest/gtest.h"

using namespace p2p;
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(500)) // give logs time to output

TEST(Listener, ping_test_ipv4)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;

    const char* port = "4000";
    Listener listener(port);
    Node node("127.0.0.1", port);

    listener.start_listening();
    PAUSE; // Give the listener thread time to start
    if (!node.ping())
    {
        FAIL() << "ping or ping response was not successful by node.";
        return;
    }
    if (!node.ping())
    {
        FAIL() << "ping or ping response was not successful by node.";
        return;
    }
    listener.stop_listening();
    PAUSE;
}

TEST(Listener, ping_test_ipv6)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;

    const char* port = "4000";
    Listener listener(port);
    Node node("::1", port);

    listener.start_listening();
    PAUSE; // Give the listener thread time to start
    if (!node.ping())
    {
    FAIL() << "ping or ping response was not successful by node.";
    return;
    }
    listener.stop_listening();
    PAUSE;
}
