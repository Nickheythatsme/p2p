//
// Created by Nick Grout on 2019-05-05.
//

#include "../src/listener.hpp"
#include "../src/node.hpp"
#include "gtest/gtest.h"

using namespace p2p;
#define PAUSE(T) std::this_thread::sleep_for(std::chrono::milliseconds(T)) // give logs time to output
#define PORT "8000"

TEST(Listener, ping_test_ipv4)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;

    try {
        Listener listener(PORT);
        Node node("127.0.0.1", PORT);
        listener.start_listening();
        PAUSE(100); // Give the listener thread time to start
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
        PAUSE(100);
    } catch(std::exception& e)
    {
        FAIL() << "Exception thrown during ping test: " << e.what();
        return;
    }
    PAUSE(100);
}

TEST(Listener, ping_test_ipv6)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;

    try {
        Listener listener(PORT, true);
        Node node("0:0:0:0:0:0:0:1", PORT);
        listener.start_listening();
    }catch (std::exception& e) {
        // Skip this test if we cannot assign ipv6 address
        logger.warn(std::string("Error listening. Probably could not assign ipv6 address on this machine") + e.what());
        PAUSE(150);
        return;
    }
    PAUSE(100);
}

TEST(Listener, SendFile)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;

    Listener listener("8081");
    Node node("127.0.0.1", "8081");

    listener.start_listening();
    PAUSE(100); // Give the listener thread time to start
    if (!node.send_file("Makefile"))
    {
        FAIL() << "ping or ping response was not successful by node.";
        return;
    }
    PAUSE(1000);
    listener.stop_listening();
    PAUSE(100);
}
