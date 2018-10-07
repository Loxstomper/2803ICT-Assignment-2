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
        printf("ADDED: %d \n", val);
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

    printf("POPPING: %d\n", pop);
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

        printf("SLOT: %d, N: %d", v.slot, v.n);
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


void init_thread_pool(Thread_Pool* tp, int capacity)
{
    (*tp).capacity = capacity;
    (*tp).used = 0;

    // create the threads
}



void thread_function()
{

}



int main(int argc, char** argv)
{
    // int n_threads = atoi(argv[1]);
    // pthread_t pool = malloc(sizeof(pthread_t) * n_threads);

    int n_threads = 4;

    Thread_Pool thread_pool;
    thread_pool.capacity = n_threads;
    thread_pool.used = 0;

    Job_Queue job_queue;

    init_queue(&job_queue, 8);

    printf("SIZE: %d \n", job_queue.size);
    printf("LENGTH: %d \n", job_queue.length);

    Job a = {1, 2};
    new_job(&job_queue, a);
    a.slot = 2;
    a.n = 4;
    new_job(&job_queue, a);

    print_jobs(&job_queue);

    return 0;
}