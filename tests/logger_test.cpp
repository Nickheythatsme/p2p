// 
// Nicholas Grout 4/30
//
#include "gtest/gtest.h"
#include "../src/util/logger.hpp"

using namespace p2p;
using namespace std;

void test_log(int x)
{
    Logger logger;

    for (int i=0; i<5; ++i) {
        string message = std::string("thread #" +
            std::to_string(x) +
            " test #" +
            std::to_string(i));
        logger.debug(
            message
        );
    }
}

TEST(Logger, Smoke) {
    Logger logger;
    std::vector<std::thread> test_threads;
    auto log_thread = std::thread(&Logger::run);

    for (int i=0; i<5; ++i)
    {
        test_threads.emplace_back(
            std::thread(test_log, i)
        );
    }
    usleep(1000000L);
    for (std::thread &test_thread : test_threads)
    {
        test_thread.join();
    }
    log_thread.join();
}

