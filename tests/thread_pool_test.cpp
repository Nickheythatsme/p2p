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

void* long_task(va_list list)
{
    int* array = va_arg(list, int*);
    size_t len = va_arg(list, size_t);

    for (int i=0; i<len; ++i)
    {
        array[i] *= 100;
    }
    return nullptr;
}

TEST(ThreadPool, basic_test)
{
    Worker worker;
    int* array = new int[200];
    array[0] = 1;
    auto size = sizeof(array) / sizeof(int);

    worker.start(long_task, array, size);
    
    PAUSE(1000);
    ASSERT_EQ(array[0], 100);
}
