#include "common.h" 
#include "server_func.h"
#include "thread_pool.h"
#include "job_queue.h"

int main(int argc, char** argv)
{
    // THE CLIENT KILLS SERVER
    signal(SIGINT, cleanup);

    int n_threads;

    if (argc > 1)
    {
        n_threads = atoi(argv[1]);
        printf("Running server with %d threads.\n", n_threads);
    }
    else 
    {
        n_threads = 320;
        printf("Running server with %d threads.\n", n_threads);
    }

    // shared memory
    Shared_Memory* shared_memory;
    create_shared_memory(&shared_memory);

    // rotations
    unsigned long int** rotations;
    build_rotations(&rotations);

    // slot mutex
    pthread_mutex_t slot_mutex[N_SLOTS];
    for (int i = 0; i < N_SLOTS; i ++)
    {
        pthread_mutex_init(&slot_mutex[i], NULL);
    }

    // how many jobs remaining per job - zeroing too
    int* remaining_jobs = (int*) calloc(sizeof(int), sizeof(int) * N_SLOTS);

    // setup threadpool and jobqueue
    Thread_Pool thread_pool;
    Job_Queue job_queue;
    Job job_to_add;

    // setup thread args
    Thread_Args thread_args;
    thread_args.shared_memory = shared_memory;
    thread_args.slot_mutex = slot_mutex;
    thread_args.job_queue = &job_queue;
    thread_args.remaining_jobs = remaining_jobs;


    init_queue(&job_queue, N_SLOTS * N_ROTATIONS);
    init_thread_pool(&thread_pool, n_threads, &job_queue, &thread_args);


    int slot_to_use;

    while (1)
    {
        // the client quits the server
        if (shared_memory->client_flag == 'q')
        {
            cleanup(0);
        }

        if (shared_memory->client_flag == 1)
        {
            // printf("Oh boy the client has sent me a new number\n");
            // printf("Number: %lu \n", shared_memory->number);

            // figure out what slot is usable
            slot_to_use = -1;
            for (int i = 0; i < N_SLOTS; i ++)
            {
                if (shared_memory->slots[i] == -1)
                {
                    slot_to_use = i;
                    break;
                }
            }

            if (slot_to_use != -1)
            {
                printf("There is an available slot and its index is: %d \n\n", slot_to_use);

            }
            // this wont happen though, because validation is performed client side
            else
            {
                // printf("There is not an available slot to use \n\n");
                shared_memory->client_flag = 0;
                continue;
            }

            // do the rotations and add all the rotations to the job queue
            rotate(rotations, slot_to_use, shared_memory->number);

            // update the remaining jobs
            remaining_jobs[slot_to_use] = 32;

            pthread_mutex_lock(&job_queue.add_mutex);
            for (int i = 0; i < N_ROTATIONS; i ++)
            {
                job_to_add.slot = slot_to_use;
                job_to_add.n = rotations[slot_to_use][i];
                new_job(&job_queue, job_to_add);
            }
            pthread_mutex_unlock(&job_queue.add_mutex);


            // slot - give intial value of 0
            shared_memory->slots[slot_to_use] = 0;
            shared_memory->server_flag[slot_to_use] = 0;
            shared_memory->number = slot_to_use;
            // printf("THE VALUE OF NUMBER NOW WHICH IS THE SLOT IS %lu \n", shared_memory->number);

            shared_memory->client_flag = 0;

        }

    }
 
    // // detach boiii
    // detach_shared_memory(&number, &client_flag, &server_flag, &slots);

    return 0;
}
