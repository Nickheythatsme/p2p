// Nick Grout 5/3/2019
#include "thread_pool.h"
#include <unistd.h>

namespace p2p {
void* sroutine(void* arg_attr);

void* init_sroutine(void* arg_attr)
{
    // Get args
    sroutine_attr* attr = (sroutine_attr*) arg_attr;

    pthread_mutex_lock(&attr->wait_mutex); // lock
    bool exit_when_done = attr->exit_when_done;
    func_ptr next_routine = attr->routine;
    void** next_args = attr->args;
    attr->routine = nullptr;
    attr->args = nullptr;
    pthread_mutex_unlock(&attr->wait_mutex); // unlock

    if (next_routine)
    {
        next_routine(next_args);
    }
    if (exit_when_done)
    {
        return nullptr;
    }
    return sroutine(arg_attr);
    
}

void* sroutine(void* arg_attr)
{
    // Get args
    sroutine_attr* attr = (sroutine_attr*) arg_attr;

    pthread_cond_wait(&attr->wait_cond, &attr->wait_mutex); // wait, lock

    bool exit_when_done = attr->exit_when_done;
    func_ptr next_routine = attr->routine;
    void** next_args = attr->args;
    attr->routine = nullptr;
    attr->args = nullptr;

    pthread_mutex_unlock(&attr->wait_mutex); // unlock

    if (exit_when_done)
    {
        pthread_exit(nullptr);
    }
    next_routine(next_args);

    return sroutine(arg_attr);
}

Worker::Worker()
{
    attr.reset(new sroutine_attr());
    init_sroutine_attr(attr.get());
    pthread_create(&thread, nullptr, init_sroutine, attr.get());
}

void Worker::init_sroutine_attr(sroutine_attr* attr)
{
    if (pthread_cond_init(&attr->wait_cond, nullptr))
    {
        perror("Error initializing wait condition");
    }
    if (pthread_mutex_init(&attr->wait_mutex, nullptr))
    {
        perror("Error initializing wait mutex");
    }
}

void Worker::destroy_sroutine_attr(sroutine_attr* attr)
{
    pthread_cond_destroy(&attr->wait_cond);
    pthread_mutex_destroy(&attr->wait_mutex);
}

void Worker::start(func_ptr routine, void** args)
{
    attr->routine = routine;
    attr->args = args;
    if (pthread_cond_signal(&attr->wait_cond))
    {
        perror("error sending cond signal");
    }
}

Worker::Worker(Worker&& rhs)
{
    attr = std::move(rhs.attr);
    thread = rhs.thread;
    rhs.thread = FAKE_THREAD;
}

Worker::~Worker()
{
    if (thread == FAKE_THREAD)
        return;

    pthread_mutex_lock(&attr->wait_mutex); // lock
    attr->exit_when_done = true;
    pthread_mutex_unlock(&attr->wait_mutex); // unlock

    pthread_cond_signal(&attr->wait_cond);
    pthread_join(thread, &ret_value);
    destroy_sroutine_attr(attr.get());
}

} //namespace p2p
