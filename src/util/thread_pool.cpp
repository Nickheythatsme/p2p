// Nick Grout 5/3/2019
#include "thread_pool.h"
#include <unistd.h>

namespace p2p {

void* init_routine(void* arg_attr)
{
    routine_attr* attr = (routine_attr*) arg_attr;
    printf("Worker: waiting!\n");
    pthread_cond_wait(&attr->wait_cond, &attr->wait_mutex);
    printf("Worker: resuming!\n");
    return attr->routine(attr->args);
}

Worker::Worker()
{
    attr = std::make_shared<struct routine_attr>();
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
    va_end(attr->args);
    if (pthread_cond_destroy(&attr->wait_cond))
    {
        perror("error destroying wait condition");
    }
    if (pthread_mutex_destroy(&attr->wait_mutex))
    {
        perror("error destroying wait mutex");
    }

}

void Worker::start(routine_ptr routine, ...)
{
    attr->routine = routine;
    va_list list;
    va_start(list, routine);
    va_copy(attr->args, list);
    if (pthread_cond_signal(&attr->wait_cond))
    {
        perror("error sending cond signal");
    }
}

Worker::~Worker()
{
    if (pthread_cond_signal(&attr->wait_cond))
    {
        perror("error sending cond signal");
    }
    usleep(1000);
    if (pthread_join(thread, nullptr))
    {
        perror("error joining thread");
    }
}

} //namespace p2p
