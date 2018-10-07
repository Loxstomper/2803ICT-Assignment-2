#ifndef SERVER_FUNC_H
#define SERVER_FUNC_H

#include "common.h"

// making this a yucky global so it can be deleted on CTRL+C
int shm_id;

void factor(unsigned long int n);

void rotate(unsigned long int** rotations, int index, unsigned long int number);
 
void build_rotations(unsigned long int*** rotations);
 
void print_rotations(unsigned long int** rotations);

void debug_print(Shared_Memory** shared_memory);

void cleanup(int param);

// attaches to shared memory and initialises the struct
void create_shared_memory(Shared_Memory** shared_memory);

#endif