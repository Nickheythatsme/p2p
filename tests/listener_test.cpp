//
// Created by Nick Grout on 2019-05-04.
//

#include "../src/listener.hpp"
#include "../src/node.hpp"
#include "gtest/gtest.h"

using namespace p2p;
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(100)) // give logs time to output

TEST(Listener, Smoke)
{
    Logger logger("Main");
    Logger::log_level = Logger::DEBUG;
    Logger::use_console();
    Listener listener;
    PAUSE;
}

TEST(Listener, handle_stop)
{
    Logger logger("Main");
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;
    Listener listener;
    logger.info("Starting listener");
    listener.start_listening(8080);

    PAUSE;
    logger.info("Stopping listener");
    listener.stop_listening();
    PAUSE;
}
