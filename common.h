#include <stdio.h>
#include <string.h>

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>

#define SHARED_MEMORY_KEY 8008
#define N_SLOTS 10
#define N_ROTATIONS 32

typedef unsigned long int uli;

int number_shmid;
int client_flag_shmid;
int server_flag_shmid;
int slots_shmid;

struct Shared_Memory
{
    uli   number;
    uli   slots[N_SLOTS];
    char  server_flag[N_SLOTS];
    char  client_flag;
} typedef Shared_Memory;

#endif