// Nick Grout 5/3/2019

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdarg.h>
#include <cstdio>
#include <memory>

namespace p2p
{
struct sroutine_attr;

using func_ptr = void* (*)(void**);
using sroutine_attr_ptr = std::shared_ptr<struct sroutine_attr>;
#define FAKE_THREAD 0

struct sroutine_attr
{
    pthread_cond_t wait_cond;
    pthread_mutex_t wait_mutex;
    func_ptr routine;
    void** args;
    bool exit_when_done {false};
};

class Worker
{
    public:
        Worker();
        Worker(const Worker& rhs) = delete;
        Worker(Worker&& rhs);
        ~Worker();
        void start(func_ptr ptr, void** args);
    protected:
    private:
        static void init_sroutine_attr(sroutine_attr* rattr);
        static void destroy_sroutine_attr(sroutine_attr* args);
        pthread_t thread;
        sroutine_attr_ptr attr;
        void* ret_value {nullptr};
};

} // namespace p2p

#include "thread_pool.cpp"
#endif // THREAD_POOL_H
