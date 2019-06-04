// Nick Grout 5/3/2019
#include "thread_pool.h"
#include <unistd.h>

namespace p2p {

void* init_routine(void* arg_attr)
{
    routine_attr* attr = (routine_attr*) arg_attr;
    if (pthread_mutex_lock(&attr->wait_mutex))
    {
        perror("error locking mutex");
    }
    bool do_wait = attr->do_wait;
    if (pthread_mutex_unlock(&attr->wait_mutex))
    {
        perror("error unlocking mutex");
    }
    if (do_wait)
    {
        printf("Worker: waiting!\n");
        pthread_cond_wait(&attr->wait_cond, &attr->wait_mutex);
    }
    printf("Worker: resuming!\n");
    if (attr->routine)
    {
        void* ret_val = attr->routine(attr->args);
        pthread_exit(ret_val);
    }
    else
    {
        pthread_exit(nullptr);
    }
}

void destroy_routine_args(routine_attr*)
{
}

Worker::Worker()
{
    attr = std::shared_ptr<struct routine_attr>(new routine_attr, &destroy_routine_args);
    init_routine_attr(attr.get());
    pthread_create(&thread, nullptr, init_routine, attr.get());
}

void Worker::init_routine_attr(routine_attr* attr)
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

void Worker::destroy_routine_attr(routine_attr* attr)
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

void Worker::start(routine_ptr routine, void** args)
{
    if (pthread_mutex_lock(&attr->wait_mutex))
    {
        perror("error locking mutex");
    }
    attr->routine = routine;
    attr->args = args;
    attr->do_wait = false;
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
    attr->do_wait = false;
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
    destroy_routine_attr(attr.get());
}

} //namespace p2p
