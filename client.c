#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NUMBER_KEY 10000
#define CLIENT_FLAG_KEY 10001   
#define SERVER_FLAG_KEY 10002   

typedef unsigned long int uli;


// check the query status and display any new info
void get_queries()
{

}

void get_shared_memory(uli** number, int** client_flag, int** server_flag)
{
    key_t key = ftok("shmfile", NUMBER_KEY);
    int shmid;
    
    if ((shmid = shmget(key, sizeof(unsigned long int), 0666 | IPC_CREAT)) == -1)
    {
        perror("SHMGET, number");
        exit(1);

    }

    *number = (unsigned long int*) shmat(shmid, (void*)0, 0);

    key = ftok("shmfile", CLIENT_FLAG_KEY);

    if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT)) == -1)
    {
        perror("SHMGET, client flag");
        exit(1);

    }

    *client_flag = (int* ) shmat(shmid, (void*)0, 0);

    key = ftok("shmfile", SERVER_FLAG_KEY);

    // if ((shmid = shmget(key, sizeof(int) * 10, 0666 | IPC_CREAT)) == -1)
    // {
    //     perror("SHMGET, server flag");
    //     exit(1);

    // }

    // *server_flag = (int* ) shmat(shmid, (void*)0, 0);
}

void detach_shared_memory(uli** number)
{
    shmdt(&number);
}


int main(int argc, char** argv)
{
    char user_input[1000];
    int n_requests = 0;

    uli* number;
    int* client_flag;
    int* server_flag;

    get_shared_memory(&number, &client_flag, &server_flag);

    *client_flag = 0;


    int used_slot;


    while (1)
    {
        printf("> ");
        fgets(user_input, 1000, stdin);

        if (user_input[0] == 'q')
        {
            printf("Goodbye ( ﾟ▽ﾟ)/ \n");
            break;
        }

        if (n_requests == 10)
        {
            printf("System is busy! \n");
            continue;
        }


        // blocking, will wait for server to read
        while (*client_flag == 1)
        {

        }

        *number =  strtoul(user_input, NULL, 0);
        *client_flag = 1;
        printf("NUMBER: %lu \n", *number);

        // wait for the server to read number
        while (*client_flag == 1)
        {

        }

        // server writes the slot the number is in into the number variable
        used_slot = *number;

    }

    // detach boiii
    detach_shared_memory(&number);

    return 0;
}
