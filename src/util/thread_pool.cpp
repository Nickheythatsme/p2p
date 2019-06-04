// Nick Grout 5/3/2019
#include "thread_pool.h"
#include <unistd.h>

namespace p2p {

void* sroutine(void* arg_attr)
{
    // Get args
    sroutine_attr* attr = (sroutine_attr*) arg_attr;
    if (pthread_mutex_lock(&attr->wait_mutex))
    {
        perror("error locking mutex");
    }
    func_ptr next_routine = attr->routine;
    void** next_args = attr->args;
    bool do_exit = attr->exit_when_done;
    attr->routine = nullptr;
    attr->args = nullptr;
    if (pthread_mutex_unlock(&attr->wait_mutex))
    {
        perror("error unlocking mutex");
    }

    if (do_exit)
    {
        pthread_exit(nullptr);
    }

    if (next_routine)
    {
        next_routine(next_args);
    }

    printf("Worker: waiting!\n");
    pthread_cond_wait(&attr->wait_cond, &attr->wait_mutex);
    printf("Worker: resuming!\n");

    sroutine(arg_attr);
}

Worker::Worker()
{
    attr.reset(new sroutine_attr());
    init_sroutine_attr(attr.get());
    pthread_create(&thread, nullptr, sroutine, attr.get());
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
    if (pthread_cond_destroy(&attr->wait_cond))
    {
        perror("error destroying wait condition");
    }
    if (pthread_mutex_destroy(&attr->wait_mutex))
    {
        perror("error destroying wait mutex");
    }
}

void Worker::start(func_ptr routine, void** args)
{
    if (pthread_mutex_lock(&attr->wait_mutex))
    {
        perror("error locking mutex");
    }
    attr->routine = routine;
    attr->args = args;
    if (pthread_mutex_unlock(&attr->wait_mutex))
    {
        perror("error unlocking mutex");
    }
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

    if (pthread_mutex_lock(&attr->wait_mutex))
    {
        perror("error locking mutex");
    }
    attr->exit_when_done = true;
    // TODO delete args
    if (pthread_mutex_unlock(&attr->wait_mutex))
    {
        perror("error unlocking mutex");
    }

    if (pthread_cond_signal(&attr->wait_cond))
    {
        perror("error sending cond signal");
    }
    if (pthread_join(thread, &ret_value))
    {
        perror("error joining thread");
    }
    destroy_sroutine_attr(attr.get());
}

} //namespace p2p
