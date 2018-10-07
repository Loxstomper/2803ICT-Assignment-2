#include "thread_pool.h"

void* thread_function(void* j_q)
{
    printf("Thread started \n");
    // this makes sense because these threads are going to be persistent
    Job_Queue* job_queue = (Job_Queue*) j_q;
    Job to_do;

    while (1)
    {
        to_do = take_job(job_queue);

        if (to_do.slot == -1 && to_do.n == -1)
        {
            // printf("EMPTY JOBS\n");
            continue;
        }
        printf("WORKING ON JOB: slot: %d n: %d \n", to_do.slot, to_do.n);
    }


}

void init_thread_pool(Thread_Pool* tp, int capacity, Job_Queue* job_queue)
{
    (*tp).capacity = capacity;
    (*tp).used = 0;
    (*tp).pool = (pthread_t*) malloc(sizeof(pthread_t) * capacity);

    // create the threads
    for (int i = 0; i < (*tp).capacity; i ++)
    {
        if (pthread_create(&(*tp).pool[i], NULL, thread_function, (void*) job_queue))
        {
            perror("Failed to create thread \n");
            exit(1);
        }

        // pthread_join((*tp).pool[i], NULL);

        // CAUSING SEG FAULTS
        // pthread_detach((*tp).pool[i]);
    }

    for (int i = 0; i < (*tp).capacity; i ++)
    {
        pthread_detach((*tp).pool[i]);
    }
}