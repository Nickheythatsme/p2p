#include "../src/routing.hpp"
#include "gtest/gtest.h"
#include <iostream>

#define PAUSE(T) std::this_thread::sleep_for(std::chrono::milliseconds(T)) // give logs time to output

using namespace p2p;

TEST(Routing, smoke)
{
    File file;
}
