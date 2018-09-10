#include <stdio.h>

void rotate(unsigned long int number)
{
    printf("ORIGINAL: %lu \n", number);

    unsigned long int rotated;

    for (int i = 0; i < 32; i ++)
    {
        rotated = (number >> i) | (number << 32 - i);
        printf("ROTATE: %d : %d \n", i, rotated);
    }
}

void build_rotations(unsigned long int* rotations)
{
    rotations = malloc(sizeof(unsigned long int*) * 10);

    for (int i = 0; i < 10; i ++)
    {
        rotations[i] = malloc(sizeof(unsigned long int) * 32);
    }
}

void print_

int main(int argc, char** argv)
{
    unsigned long int number;

    unsigned long int* rotations;
    build_rotations(&rotations);



    number = 4;

    rotate(number);

    return 0;
}
