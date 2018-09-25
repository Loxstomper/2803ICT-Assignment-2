#include <stdio.h>
#include <string.h>
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define NUMBER_KEY 10000
#define CLIENT_FLAG_KEY 10001   
#define SERVER_FLAG_KEY 8008135   

#define N_SLOTS 10
#define N_ROTATIONS 32

typedef unsigned long int uli;

void get_shared_memory(uli** number, int** client_flag, int** server_flag);
void detach_shared_memory(uli** number, int** client_flag, int** server_flag);

#endif