#include "common.h"

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

    if ((shmid = shmget(SERVER_FLAG_KEY, sizeof(int*) * 10, 0666 | IPC_CREAT)) == -1)
    {
        perror("SHMGET, server flag");
        printf("THE ERROR IS: %s\n", strerror(errno));
        exit(1);

    }

    *server_flag = (int* ) shmat(shmid, (void*)0, 0);
}

void detach_shared_memory(uli** number)
{
    shmdt(&number);
}