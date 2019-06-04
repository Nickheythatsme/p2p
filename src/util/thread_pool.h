// Nick Grout 5/3/2019

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdarg.h>
#include <cstdio>
#include <memory>

namespace p2p
{
struct routine_attr;

using init_routine_ptr = void* (*)(struct routine_attr*);
using routine_ptr = void* (*)(void**);
using attr_ptr = std::shared_ptr<struct routine_attr>;
#define FAKE_THREAD 0

struct routine_attr
{
    pthread_cond_t wait_cond;
    pthread_mutex_t wait_mutex;
    routine_ptr routine;
    bool do_wait {true};
    void** args;
};

void destroy_routine_args(routine_attr*);

class Worker
{
    public:
        Worker();
        Worker(const Worker& rhs) = delete;
        Worker(Worker&& rhs);
        ~Worker();
        void start(routine_ptr, void** args);
    protected:
    private:
        static void init_routine_attr(routine_attr* rattr);
        static void destroy_routine_attr(routine_attr* args);
        pthread_t thread;
        attr_ptr attr;
        void* ret_value {nullptr};
};

} // namespace p2p

#include "thread_pool.cpp"
#endif // THREAD_POOL_H
