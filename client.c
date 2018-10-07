#include "common.h"



int shm_id;

// had to make global so the CTRL+C event cant modify the shared memory to shutdown server
Shared_Memory* shared_memory;

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

    signal(SIGINT, cleanup);

    char user_input[1000];
    int n_requests = 0;


    int used_slot;

    connect_shared_memory(&shared_memory);

    int i = 0;

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
        shared_memory->client_flag = 1;

        // blocking, will wait for server to read
        while (shared_memory->client_flag == 1) {}

        // server writes the slot the number is in into the number variable
        // printf("Just got a response from the server ");
        printf("USING SLOT (number): %lu\n", shared_memory->number);

        int slot = shared_memory->number;
        uli last;
        uli update;

        // shared_memory->server_flag[0] = 0;

        while (shared_memory->server_flag[0] != 'f')
        {
            // waits until server_flag is 1
            while (shared_memory->server_flag[slot] == 0) {}
            printf("NEW FACTOR!: %lu\n", shared_memory->slots[slot]);
            shared_memory->server_flag[slot] = 0;
        }
    }

    return 0;
}