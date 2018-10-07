#include "common.h"
#include <sys/time.h>
#include <pthread.h>



struct Query
{
    unsigned long int n;
    unsigned long int* slots;
    int slot;
    char* server_flag;
    float* progress;
    struct timeval start_time;
} typedef Query;


int shm_id;
struct timeval last_update;
pthread_mutex_t progress_mutex = PTHREAD_MUTEX_INITIALIZER;

// had to make global so the CTRL+C event cant modify the shared memory to shutdown server
Shared_Memory* shared_memory;


void* watch(void* arg)
{
    Query* query = (Query*) arg;
    struct timeval end_time, current;
    double duration;
    int printed_progress;
    // printf("THREAD CREATED \n");

    while (1)
    {

        // get current time, 
        gettimeofday(&current, NULL);

        // 5 seconds just for testing
        pthread_mutex_lock(&progress_mutex);
        if ((current.tv_sec - last_update.tv_sec) >= 1)
        {
            printed_progress = 0;
            for (int i = 0; i < N_SLOTS; i ++)
            {
                if (query->server_flag[i] != 'f')
                {
                    if (!printed_progress)
                    {
                        printf("Progress: ");
                        printed_progress = 1;
                       
                    }
                    // + 1 because not 0 index
                    printf("Q%d:%.2f%% ", i + 1, query->progress[i]);
                }
            }

            if (printed_progress)
            {
                printf("\n");
            }
            // now pretend its been updated
            last_update.tv_sec = current.tv_sec;

        }
        pthread_mutex_unlock(&progress_mutex);



        // not being used
        if (query->slot == -1)
        {
            continue;
        }

        if (query->server_flag[query->slot] == 1)
        {
            printf("Query %d: factor %lu \n", query->slot, query->slots[query->slot]);
            // tell server data was recieved
            query->server_flag[query->slot] = 0;
            
            // update our global var of the time of the last update
            gettimeofday(&last_update, NULL);
        }

        if (query->server_flag[query->slot] == 'f')
        {
            printf("Query complete for %lu \n", query->n);
            gettimeofday(&end_time, NULL);


            duration = end_time.tv_sec - query->start_time.tv_sec + (end_time.tv_usec - query->start_time.tv_usec);

            printf("Time taken: %.6f \n", duration / 1000000.0);
            // query->server_flag[query->slot] == 'g';

            // stop the thread from doing more
            query->slot = -1;
        }
    }
}


void cleanup(int param)
{
    // dont need to detach because program is exiting and the memory going to be destroyed
    printf("\nCleaning up.....\n");
    shared_memory->client_flag = 'q';
    printf("Done\n");
    exit(0);
}


void get_server_flags(char* server_flag)
{
    for (int i = 0; i < 10; i ++)
    {
        printf("%d\t", server_flag[i]);
    }

    printf("\n");
}

int is_available_slot(uli* slots)
{
    for (int i = 0; i < N_SLOTS; i ++)
    {
        if (slots[i] == -1)
        {
            return 1;
        }
    }

    return 0;
}

void connect_shared_memory(Shared_Memory** shared_memory)
{
    key_t key = ftok("shmfile", SHARED_MEMORY_KEY);
    shm_id = shmget(key, sizeof(Shared_Memory), 0666);

    // validate that it created
    if (shm_id < 0)
    {
        perror("Unable to connect to shared memory. Is the server running?");
        exit(1);
    }

    // update pointer to the shared memory
    *shared_memory = (Shared_Memory*) shmat(shm_id, NULL, 0);
}


// maybe share a semaphore so we know if the server is full
// maybe have one thread working in the background printing the response from server
int main(int argc, char** argv)
{
    gettimeofday(&last_update, NULL);
    signal(SIGINT, cleanup);

    char user_input[1000];
    int n_requests = 0;

    unsigned long int number;



    int used_slot;

    connect_shared_memory(&shared_memory);


    // init the queries
    Query queries[N_SLOTS];
    for (int i = 0; i < N_SLOTS; i ++)
    {
        queries[i].server_flag = shared_memory->server_flag;
        queries[i].slots = shared_memory->slots;
        queries[i].slot = -1;
        queries[i].progress = shared_memory->progress;
    }

    // client threads
    pthread_t threads[N_SLOTS];

    for (int i = 0; i < N_SLOTS; i ++)
    {
        pthread_create(&threads[i], NULL, &watch, &queries[i]);
    }
    // detach
    for (int i = 0; i < N_SLOTS; i ++)
    {
        pthread_detach(threads[i]);
    }

    while (1)
    {
        printf("> ");

        fgets(user_input, 1000, stdin);

        if (user_input[0] == 'q')
        {
            // tells the server to go die
            shared_memory->client_flag = 'q';
            printf("Goodbye ( ﾟ▽ﾟ)/ \n");
            cleanup(1);
        }

        if (!is_available_slot(shared_memory->slots))
        {
            printf("Server is busy!\n");
            continue;
        }

        // blank input
        if (strlen(user_input) == 1)
        {
            continue;
        }


        shared_memory->number = strtoul(user_input, NULL, 0);
        number = shared_memory->number;
        shared_memory->client_flag = 1;

        // blocking, will wait for server to read
        while (shared_memory->client_flag == 1) {}

        // server writes the slot the number is in into the number variable
        // printf("Just got a response from the server ");
        printf("USING SLOT (number): %lu\n", shared_memory->number);

        // updating the struct for the corresponding thread
        int slot = shared_memory->number;

        // reset the progress
        shared_memory->progress[slot] = 0.0f;
        queries[slot].slot = slot;
        queries[slot].n = number;
        queries[slot].start_time;
        gettimeofday(&queries[slot].start_time, NULL);

        // consider this an update
        last_update.tv_sec = queries[slot].start_time.tv_sec;

        // uli last;
        // uli update;

        // // shared_memory->server_flag[0] = 0;

        // while (shared_memory->server_flag[0] != 'f')
        // {
        //     // waits until server_flag is 1
        //     while (shared_memory->server_flag[slot] == 0) {}
        //     printf("NEW FACTOR!: %lu\n", shared_memory->slots[slot]);
        //     shared_memory->server_flag[slot] = 0;
        // }

        // printf("Complete \n");
    }

    return 0;
}