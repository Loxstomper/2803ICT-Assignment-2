#include "common.h" 
#include "server_func.h"

// NEED TO DELETE THE SHARED MEMORY, shmctl  
int main(int argc, char** argv)
{
    unsigned long int** rotations;
    // mallocs the 2d array
    build_rotations(&rotations);

    uli* number;
    int* client_flag;
    int* server_flag;
    int* slots;

    get_shared_memory(&number, &client_flag, &server_flag, &slots, 1);

    // number = 4;
    // int index = 0;
 
    // rotate(rotations, index, number);
    // print_rotations(rotations);

    // factor(rotations[0][0]);

    while (1)
    {
        if (*client_flag == 1)
        {
            printf("Oh boy the client has sent me a new number\n");
            printf("Number: %lu \n", *number);
            // slot
            *number += 10; // just testing this
            slots[0] = 1337;
            *client_flag = 0;

            // this is where we would create thread

            uli l = 1 << 8;
            factor(l);
        }

    }
 
    // detach boiii
    detach_shared_memory(&number, &client_flag, &server_flag, &slots);

    return 0;
}
