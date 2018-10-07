#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


struct Thread_Pool 
{
    unsigned int capacity;
    unsigned int used;
    pthread_t* pool;

} typedef Thread_Pool;

struct Job 
{
    int slot;
    int n;

} typedef Job;

struct Job_Queue
{
    int head;
    int tail;
    int size;
    int length;
    // int* jobs;
    Job* jobs;
} typedef Job_Queue;


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

        printf("SLOT: %d, N: %d \n", v.slot, v.n);
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
}

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






int main(int argc, char** argv)
{
    // int n_threads = atoi(argv[1]);
    // pthread_t pool = malloc(sizeof(pthread_t) * n_threads);

    int n_threads = 4;

    Thread_Pool thread_pool;

    Job_Queue job_queue;

    init_queue(&job_queue, 320);

    // printf("SIZE: %d \n", job_queue.size);
    // printf("LENGTH: %d \n", job_queue.length);



    // lets get the thread pool working
    init_thread_pool(&thread_pool, n_threads, &job_queue);
    printf("FINISHED MAKING THREADS\n");

    // adds all the jobs
    Job job;
    for (int i = 0; i < 4; i ++)
    {
        job.slot = i;
        for (int j = 0; j < 4; j ++)
        {
           job.n = j; 

            // pass by value
           new_job(&job_queue, job);
        }
    }
    // print_jobs(&job_queue);

    while (1)
    {

    }

    return 0;
}