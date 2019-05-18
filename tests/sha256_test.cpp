//
// Created by Nick Grout on 2019-05-18.
//

#include "../src/sha256.h"
#include "gtest/gtest.h"

using namespace p2p;
#define PAUSE(T) std::this_thread::sleep_for(std::chrono::milliseconds(T)) // give logs time to output

TEST(sha256, smoke)
{
    sha256 s;
}
