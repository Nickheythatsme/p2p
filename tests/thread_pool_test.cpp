//
// Created by Nick Grout on 6/3/2019.
//

#include "../src/util/thread_pool.h"
#include "gtest/gtest.h"
#include <ctime>

using namespace p2p;
#define PAUSE(T) usleep(T);

TEST(ThreadPool, smoke_test)
{
    {
        Worker worker;
        PAUSE(1000);
    }
    PAUSE(1000);
}
