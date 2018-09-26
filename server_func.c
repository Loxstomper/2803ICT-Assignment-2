#include "server_func.h"

void factor(uli n)
{
    static uli i = 0;
    static int progress = 0;
    // five precent
     uli progess_update = (n / 20);
     printf("%lu, 1% = %lu \n", n, progess_update);
    //  exit(1);

    for (i = 1; i <= n; i ++)
    {
        if (n % i == 0)
        {
            // printf("[+] Number: %lu \t Factor: %lu \n", n, i);
        }

        if (i % progess_update == 0 && i != n)
        {
            printf("Number: %lu Progress: %d \n", n, 5 * (i / progess_update));
        }
    }

    printf("Done\n");
}

void rotate(unsigned long int** rotations, int index, unsigned long int number)
{
    static unsigned long int rotated;
 
    for (int i = 0; i < N_ROTATIONS; i ++)
    {
        rotated = (number >> i) | (number << 32 - i);
        rotations[index][i] = rotated;
    }
}
 
 
void build_rotations(unsigned long int*** rotations)
{
    (*rotations) = malloc(sizeof(unsigned long int*) * N_SLOTS);
 
    if (rotations == NULL)
    {
        perror("Failed to malloc rotations\n");
        exit(1);
    }
 
    for (int i = 0; i < N_SLOTS; i ++)
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
    for (int i = 0; i < N_SLOTS; i ++)
    {
        printf("\n[%d] \n", i);
 
        for (int j = 0; j < N_ROTATIONS; j ++)
        {
            printf("%d\t%lu \n", j, rotations[i][j]);
        }
    }
}