//
// Created by Nick Grout on 2019-05-04.
//

#include "../src/listener.hpp"
#include "gtest/gtest.h"

using namespace p2p;

TEST(Listener, Smoke)
{
    Logger logger("Main");
    Logger::use_console();
    Listener listener;
}

TEST(Listener, handle_stop)
{
    Logger logger("Main");
    Logger::use_console();
    Listener listener;
    logger.info("Starting listening");
    listener.start_listening(8080);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    logger.info("Stopping listening");
    listener.stop_listening();
}
