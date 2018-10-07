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

    // setup threadpool and jobqueue
    Thread_Pool thread_pool;
    Job_Queue job_queue;

    init_queue(&job_queue, N_SLOTS * N_ROTATIONS);
    init_thread_pool(&thread_pool, n_threads, &job_queue);



    unsigned long int** rotations;
    // mallocs the 2d array
    build_rotations(&rotations);

    Shared_Memory* shared_memory;

    create_shared_memory(&shared_memory);


    int slot_to_use;
    // number = 4;
    // int index = 0;
 
    // rotate(rotations, index, number);
    // print_rotations(rotations);

    // factor(rotations[0][0]);

    // make all the slots value to be -1 "empty / finished"
    // for (int i = 0; i < N_SLOTS; slots[i] = -1, i ++) {}


    while (1)
    {
        // the client quits the server
        if (shared_memory->client_flag == 'q')
        {
            cleanup(0);
        }

        if (shared_memory->client_flag == 1)
        {
            printf("Oh boy the client has sent me a new number\n");
            printf("Number: %lu \n", shared_memory->number);

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
                // printf("There is an available slot and its index is: %d \n\n", slot_to_use);
            }
            else
            {
                // printf("There is not an available slot to use \n\n");
                shared_memory->client_flag = 0;
                continue;
            }

            // slot
            shared_memory->slots[slot_to_use] = 69;
            shared_memory->number = slot_to_use;
            // printf("THE VALUE OF NUMBER NOW WHICH IS THE SLOT IS %lu \n", shared_memory->number);

            shared_memory->client_flag = 0;

            // this is where we would create thread
            // for (int i = 0; i < 5; i ++)
            // {
            //     // the client will change value to 0 when its been read
            //     while (shared_memory->server_flag[0] == 1) {}

            //     // update value
            //     shared_memory->slots[0] = i;
            //     // signal to client that an update has occured
            //     shared_memory->server_flag[0] = 1;
            // }

            // printf("DEBUG PRINT\n\n");

            // debug_print(&shared_memory);

            uli l = 1 << 8;
            // factor(l);
        }

    }
 
    // // detach boiii
    // detach_shared_memory(&number, &client_flag, &server_flag, &slots);

    return 0;
}
