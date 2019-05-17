//
// Created by Nick Grout on 2019-05-04.
//

#include "../src/listener.hpp"
#include "../src/node.hpp"
#include "gtest/gtest.h"

using namespace p2p;
#define PAUSE(x) std::this_thread::sleep_for(std::chrono::milliseconds(x)) // give logs time to output

TEST(Listener, Smoke)
{
    Logger logger("Main");
    Logger::log_level = Logger::DEBUG;
    Logger::use_console();
    Listener listener("8080");
    PAUSE(100);
}

TEST(Listener, handle_stop)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;

    Listener listener("8080");
    logger.info("Starting listener");
    listener.start_listening();

    PAUSE(100);
    logger.info("Stopping listener");
    listener.stop_listening();
    PAUSE(100);
}
