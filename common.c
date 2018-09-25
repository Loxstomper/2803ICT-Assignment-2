#include "common.h"

void get_shared_memory(uli** number, int** client_flag, int** server_flag, int** slots, int is_server)
{
    key_t key = ftok("shmfile", NUMBER_KEY);
    int shmid;
    
    if (is_server)
    {
        if ((shmid = shmget(key, sizeof(uli), 0666 | IPC_CREAT)) == -1)
        {
            perror("shmget, number");
            exit(1);
        }
    }
    else
    {
        if ((shmid = shmget(key, sizeof(uli), 0666)) == -1)
        {
            perror("shmget, number");
            exit(1);
        }
    }

    *number = (uli*) shmat(shmid, (void*)0, 0);

    key = ftok("shmfile", CLIENT_FLAG_KEY);

    if (is_server)
    {
        if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT)) == -1)
        {
            perror("SHMGET, client flag");
            exit(1);
        }
    }
    else
    {
        if ((shmid = shmget(key, sizeof(int), 0666 )) == -1)
        {
            perror("SHMGET, client flag");
            exit(1);
        }
    }

    *client_flag = (int* ) shmat(shmid, (void*)0, 0);

    key = ftok("shmfile", SERVER_FLAG_KEY);

    if (is_server)
    {
        if ((shmid = shmget(SERVER_FLAG_KEY, sizeof(int*) * N_SLOTS, 0666 | IPC_CREAT)) == -1)
        {
            perror("SHMGET, server flag");
            exit(1);
        }
    }
    else
    {
        if ((shmid = shmget(SERVER_FLAG_KEY, sizeof(int*) * N_SLOTS, 0666)) == -1)
        {
            perror("SHMGET, server flag");
            exit(1);
        }
    }

    *server_flag = (int* ) shmat(shmid, (void*)0, 0);

    key = ftok("shmfile", SLOTS_KEY);

    if (is_server)
    {
        if ((shmid = shmget(SLOTS_KEY, sizeof(int*) * N_SLOTS, 0666 | IPC_CREAT)) == -1)
        {
            perror("SHMGET, slots");
            exit(1);
        }
    }
    else
    {
        if ((shmid = shmget(SLOTS_KEY, sizeof(int*) * N_SLOTS, 0666)) == -1)
        {
            perror("SHMGET, slots");
            exit(1);
        }
    }

    *slots = (int* ) shmat(shmid, (void*)0, 0);
}

void detach_shared_memory(uli** number, int** client_flag, int** server_flag, int** slots)
{
    shmdt(&number);
    shmdt(&client_flag);
    shmdt(&server_flag);
    shmdt(&slots);
}