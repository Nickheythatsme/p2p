//
// Created by Nick Grout on 6/3/2019.
//

#include "../src/util/thread_pool.h"
#include "gtest/gtest.h"
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <vector>

using namespace p2p;
#define PAUSE(T) usleep(T*1000000);

TEST(ThreadPool, smoke_test)
{
    {
        // Worker worker;
    }
}

using callback_ptr = void* (*)(void** args);
void* long_task(void** args)
{
    size_t len = (size_t) args[0];
    int* array = (int*) args[1];
    callback_ptr callback = (callback_ptr) args[2];
    printf("long task: starting\n");

    for (int i=0; i<len; ++i)
    {
        array[i] = 100;
    }
    return callback(args);
}

void* callback(void** args)
{
    size_t len = (size_t) args[0];
    int* array = (int*) args[1];
    printf("callback: starting\n");
    assert(array[0] == 100);
    printf("callback: array[0] == %d\n", array[0]);
}

TEST(ThreadPool, basic_test)
{
    Worker worker;
    void** args = (void**) malloc(sizeof(void*) * 3);

    int array[100];
    array[0] = 1;
    auto size = sizeof(array) / sizeof(int);


    args[0] = malloc(sizeof(size_t));
    memcpy(&args[0], &size, sizeof(size_t));

    args[1] = malloc(sizeof(int*));
    args[1] = (void*)array;

    args[2] = (void*)callback;
    // memcpy(&args[1], &array, sizeof(int*));

    worker.start(long_task, args);
}

TEST(ThreadPool, many_threads)
{
    std::vector<Worker> workers;
    for (int i=0; i<20; ++i)
    {
        printf("Making worker %d\n", i);
        workers.emplace_back(Worker());
    }
}

TEST(ThreadPool, thread_cycle)
{
}
