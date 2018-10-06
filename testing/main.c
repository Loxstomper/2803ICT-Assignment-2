#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


struct Thread_Pool 
{
    unsigned int capacity;
    unsigned int used;
    pthread_t* pool;

} typedef Thread_Pool;

struct Job_Queue
{
    int head;
    int tail;
    int size;
    int length;
    int* jobs;
} typedef Job_Queue;

int new_job(Job_Queue* job_queue, int val)
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

int take_job(Job_Queue* job_queue)
{
    if ( (*job_queue).size = 0 )
    {
        return 0;
    }

    int pop;

    pop = (*job_queue).jobs[(*job_queue).head];

    printf("POPPING: %d\n", pop);
    (*job_queue).jobs[(*job_queue).head] = -1;

    (*job_queue).head = ((*job_queue).head + 1) % (*job_queue).length;
    (*job_queue).size --;

    return pop;
}

void print_jobs(Job_Queue* job_queue)
{
    int v;
    for (int i = 0; i < (*job_queue).size; i ++)
    {
        v = (*job_queue).jobs[ ((*job_queue).head + i) % (*job_queue).length ];

        printf(" %d, ", v);
    }
    
    printf("\n");
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
    job_queue.head = 0;
    job_queue.tail = -1;
    job_queue.size = 0;
    job_queue.length = 8;
    job_queue.jobs = (int*) calloc(sizeof(int), sizeof(int) * job_queue.length);

    new_job(&job_queue, 5);
    new_job(&job_queue, 2);
    new_job(&job_queue, 1);

    printf("SIZE: %d \n", job_queue.size);
    printf("LENGTH: %d \n", job_queue.length);


    print_jobs(&job_queue);


    for (int i = 0; i < job_queue.length; i ++)
    {
        printf("%d, ", job_queue.jobs[i]);
    }

    printf("\n");

    take_job(&job_queue);
    print_jobs(&job_queue);

    for (int i = 0; i < job_queue.length; i ++)
    {
        printf("%d, ", job_queue.jobs[i]);
    }

    printf("\n");

    take_job(&job_queue);
    for (int i = 0; i < job_queue.length; i ++)
    {
        printf("%d, ", job_queue.jobs[i]);
    }

    printf("\n");











    return 1;
}