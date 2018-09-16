
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/ipc.h>
#include <sys/shm.h>
 
#define N_ROTATIONS 32
#define N_NUMBERS 10
 
 
void factor(unsigned long int n)
{
    static unsigned long int i = 0;
    unsigned long int range = n / 2;

    for (i = 1; i <= range; i ++)
    {
        if (n % i == 0)
        {
            printf("[+] %lu \n", i);
        }
    }
}

void rotate(unsigned long int** rotations, int index, unsigned long int number)
{
    printf("ORIGINAL: %lu \n", number);
 
    static unsigned long int rotated;
 
    for (int i = 0; i < N_ROTATIONS; i ++)
    {
        rotated = (number >> i) | (number << 32 - i);
        printf("ROTATE: %d : %lu \n", i, rotated);
        rotations[index][i] = rotated;
    }
}
 
 
void build_rotations(unsigned long int*** rotations)
{
    (*rotations) = malloc(sizeof(unsigned long int*) * N_NUMBERS);
 
    if (rotations == NULL)
    {
        perror("Failed to malloc rotations\n");
        exit(1);
    }
 
    for (int i = 0; i < N_NUMBERS; i ++)
    {
        (*rotations)[i] = malloc(sizeof(unsigned long int) * N_ROTATIONS);
       
        if ((*rotations)[i] == NULL)
        {
            perror("Failed to malloc rotations 2d \n");
            exit(1);
        }
    }
}
 
 
void print_rotations(unsigned long int** rotations)
{
    for (int i = 0; i < N_NUMBERS; i ++)
    {
        printf("\n[%d] \n", i);
 
        for (int j = 0; j < N_ROTATIONS; j ++)
        {
            printf("%d\t%lu \n", j, rotations[i][j]);
        }
    }
}
 
 
int main(int argc, char** argv)
{
    unsigned long int number;
 
    unsigned long int** rotations;
    build_rotations(&rotations);
 
 
    key_t key = ftok("shmfile", 1337);
    int shmid = shmget(key, sizeof(unsigned long int), 0666 | IPC_CREAT);
    unsigned long int* shared_number;

    number = 4;
    int index = 0;
 
    rotate(rotations, index, number);
    // print_rotations(rotations);

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
