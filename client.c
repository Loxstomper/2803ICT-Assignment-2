#include "common.h"



// check the query status and display any new info
void get_queries()
{

}

void get_server_flags(int* server_flag)
{
    for (int i = 0; i < 10; i ++)
    {
        printf("%d\t", server_flag[i]);
    }

    printf("\n");
}


// use a signal to cleanup - bit difficult without global vars 


// maybe share a semaphore so we know if the server is full
int main(int argc, char** argv)
{
    char user_input[1000];
    int n_requests = 0;

    uli* number;
    int* client_flag;
    int* server_flag;
    int* slots;

    get_shared_memory(&number, &client_flag, &server_flag, &slots, 1);

    *client_flag = 0;

    int used_slot;

    while (1)
    {
        printf("> ");
        fgets(user_input, 1000, stdin);

        if (user_input[0] == 'q')
        {
            printf("Goodbye ( ﾟ▽ﾟ)/ \n");
            break;
        }

        if (n_requests == 10)
        {
            printf("System is busy! \n");
            continue;
        }

        *number =  strtoul(user_input, NULL, 0);
        printf("GOT :  %lu  \n", strtoul(user_input, NULL, 0));
        printf("SAVE: %lu \n", *number);
        *client_flag = 1;
        slots[1] = 696969696;

        // blocking, will wait for server to read
        while (*client_flag == 1) {}

        // server writes the slot the number is in into the number variable
        printf("USING SLOT: %lu\n", *number);
        printf("TEST: %d \n", slots[0]);


        get_server_flags(server_flag);
    }

    // detach boiii
    detach_shared_memory(&number, &client_flag, &server_flag, &slots);

    return 0;
}