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


TEST(Listener, ping)
{
    Logger logger("Main");
    Logger::use_console();
    Listener listener;
    listener.start_listening(8080);
}

