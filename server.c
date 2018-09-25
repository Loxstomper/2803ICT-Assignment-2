#include "common.h" 
#include "server_func.h"
 
int main(int argc, char** argv)
{
    // unsigned long int number;
    unsigned int number;
 
    unsigned long int** rotations;
    build_rotations(&rotations);
 
 
    key_t key = ftok("shmfile", NUMBER_KEY);
    int shmid = shmget(key, sizeof(unsigned long int), 0666 | IPC_CREAT);
    unsigned long int* shared_number;

    number = 4;
    int index = 0;
 
    rotate(rotations, index, number);
    print_rotations(rotations);
    exit(1);

    // factor(rotations[0][0]);

    while (1)
    {
        shared_number = (unsigned long int*) shmat(shmid, (void*)0, 0);
        printf("%lu \n", *shared_number);


    }
    // detach boiii
    shmdt(shared_number);
 
    return 0;
}
