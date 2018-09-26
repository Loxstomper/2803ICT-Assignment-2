#include "common.h" 
#include "server_func.h"


void cleanup(int param)
{
    printf("\nCleaning up.....\n");
    shmctl(number_shmid,        IPC_RMID, 0);
    shmctl(client_flag_shmid,   IPC_RMID, 0);
    shmctl(server_flag_shmid,   IPC_RMID, 0);
    shmctl(slots_shmid,         IPC_RMID, 0);
    printf("Done\n");
    exit(1);
}

// NEED TO DELETE THE SHARED MEMORY, shmctl  
int main(int argc, char** argv)
{

    signal(SIGINT, cleanup);

    uli* number;
    int* client_flag;
    int* server_flag;
    int* slots;

    unsigned long int** rotations;
    // mallocs the 2d array
    build_rotations(&rotations);


    get_shared_memory(&number, &client_flag, &server_flag, &slots, 1);

    int slot_to_use;
    // number = 4;
    // int index = 0;
 
    // rotate(rotations, index, number);
    // print_rotations(rotations);

    // factor(rotations[0][0]);

    // make all the slots value to be -1 "empty / finished"
    for (int i = 0; i < N_SLOTS; slots[i] = -1, i ++) {}


    while (1)
    {
        if (*client_flag == 1)
        {
            printf("Oh boy the client has sent me a new number\n");
            printf("Number: %lu \n", *number);

            // figure out what slot is usable
            slot_to_use = -1;
            for (int i = 0; i < N_SLOTS; i ++)
            {
                if (slots[i] == -1)
                {
                    slot_to_use = i;
                    break;
                }
            }

            if (slot_to_use != -1)
            {
                printf("There is an available slot and its index is: %d \n\n", slot_to_use);
            }
            else
            {
                printf("There is not an available slot to use \n\n");
                *client_flag = 0;
                continue;
            }

            // slot
            slots[slot_to_use] = 69;
            *number = slot_to_use;
            printf("THE VALUE OF NUMBER NOW WHICH IS THE SLOT IS %lu \n", *number);

            *client_flag = 0;

            // this is where we would create thread

            uli l = 1 << 8;
            // factor(l);
        }

    }
 
    // detach boiii
    detach_shared_memory(&number, &client_flag, &server_flag, &slots);

    return 0;
}
