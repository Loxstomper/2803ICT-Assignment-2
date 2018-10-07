#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

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


int new_job(Job_Queue* job_queue, Job val);

Job take_job(Job_Queue* job_queue);

void print_jobs(Job_Queue* job_queue);

void init_queue(Job_Queue* queue, int length);

#endif