#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "job_queue.h"
#include <stdlib.h>
#include <pthread.h>

struct Thread_Pool 
{
    unsigned int capacity;
    unsigned int used;
    pthread_t* pool;

} typedef Thread_Pool;

// probably change this to be able to pass in a function pointer as an argument
void* thread_function(void* j_q);

void init_thread_pool(Thread_Pool* tp, int capacity, Job_Queue* job_queue);

#endif