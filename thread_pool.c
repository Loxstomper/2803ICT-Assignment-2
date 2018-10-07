#include "thread_pool.h"

void* thread_function(void* j_q)
{
    printf("Thread started \n");
    // this makes sense because these threads are going to be persistent
    Job_Queue* job_queue = (Job_Queue*) j_q;
    Job to_do;

    // not sure if i need to use mutex
    while (1)
    {
        to_do.slot = -1;
        // MUTEX LOCK
        pthread_mutex_lock(&job_queue->pop_mutex);

        if (job_queue->size > 1)
        {
            to_do = take_job(job_queue);
        }

        // MUTEX UNLOCK
        pthread_mutex_unlock(&job_queue->pop_mutex);

        // this shouldnt happen but just in case
        if (to_do.slot == -1)
        {
            continue;
        }



        // if (to_do.slot == -1 && to_do.n == -1)
        // {
        //     // printf("EMPTY JOBS\n");
        //     continue;
        // }
        printf("WORKING ON JOB: slot: %d n: %d \n", to_do.slot, to_do.n);

        int progress = 0;
        // five precent
        // unsigned long int progess_update = (to_do.n / 20);
        // printf("%lu, 1% = %lu \n", to_do.n, progess_update);
        //  exit(1);

        for (unsigned long int i = 1; i <= to_do.n; i ++)
        {
            if (to_do.n % i == 0)
            {
                // printf("[+] Number: %lu \t Factor: %lu \n", n, i);
            }

            // if (i % progess_update == 0 && i != to_do.n)
            // {
            //     progress = 5 * (i / progess_update);
            //     printf("Number: %lu Progress: %d \n", to_do.n, progress); 
            // }
        }

        printf("Done\n");
    
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