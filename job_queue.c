#include "job_queue.h"

int new_job(Job_Queue* job_queue, Job val)
{
    if ( (*job_queue).size < (*job_queue).length )
    {
        // printf("ADDED: %d \n", val);
        // calculate position of new tail (with wrapping)
        (*job_queue).tail = ((*job_queue).tail + 1) % (*job_queue).length;
        // (*job_queue).tail --; 
        // put the val in current tail pos
        (*job_queue).jobs[(*job_queue).tail] = val;
        // update the size of the queue
        (*job_queue).size ++;

        return 1;
    }
    return 0;
}

Job take_job(Job_Queue* job_queue)
{
    if ( (*job_queue).size == 0 )
    {
        Job a = {-1, -1};
        return a;
    }

    Job pop;

    pop = (*job_queue).jobs[(*job_queue).head];

    // printf("POPPING: %d\n", pop);
    // (*job_queue).jobs[(*job_queue).head] = -1;

    (*job_queue).head = ((*job_queue).head + 1) % (*job_queue).length;
    (*job_queue).size --;

    return pop;
}

void print_jobs(Job_Queue* job_queue)
{
    Job v;
    for (int i = 0; i < (*job_queue).size; i ++)
    {
        v = (*job_queue).jobs[ ((*job_queue).head + i) % (*job_queue).length ];

        printf("SLOT: %d, N: %lu \n", v.slot, v.n);
    }
    
    printf("\n");
}

void init_queue(Job_Queue* queue, int length)
{
    (*queue).head = 0;
    (*queue).tail = -1;
    (*queue).size = 0;
    (*queue).length = length;
    // (*queue).jobs = (int*) calloc(sizeof(int), sizeof(int) * length);
    (*queue).jobs = (Job*) calloc(sizeof(Job), sizeof(Job) * length);

    if ((*queue).jobs == NULL)
    {
        perror("Calloc failed in job queue");
    }

    // NOT WORKING IN STRUCT
    // (*queue).pop_mutex = PTHREAD_MUTEX_INITIALIZER;
    // (*queue).add_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_init(&(*queue).pop_mutex, NULL);
    pthread_mutex_init(&(*queue).add_mutex, NULL);
}