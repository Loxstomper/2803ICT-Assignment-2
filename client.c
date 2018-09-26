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

int is_available_slot(int* slots)
{
    for (int i = 0; i < N_SLOTS; i ++)
    {
        if (slots[i] == -1)
        {
            return 1;
        }
    }

    return 0;
}


// use a signal to cleanup - bit difficult without global vars 
// the cleanup only really needs to be done on the server


// maybe share a semaphore so we know if the server is full
// maybe have one thread working in the background printing the response from server
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

    printf("GLOBAL TEST: %d\n", global_test);

    while (1)
    {
        printf("> ");

        fgets(user_input, 1000, stdin);

        if (!is_available_slot(slots))
        {
            printf("Server is busy!\n");
            continue;
        }

        // blank input
        if (strlen(user_input) == 1)
        {
            continue;
        }

        if (user_input[0] == 'q')
        {
            printf("Goodbye ( ﾟ▽ﾟ)/ \n");
            break;
        }

        *number =  strtoul(user_input, NULL, 0);
        *client_flag = 1;

        // blocking, will wait for server to read
        while (*client_flag == 1) {}


        // server writes the slot the number is in into the number variable
        printf("Just got a response from the server ");
        printf("USING SLOT (number): %lu\n", *number);

        // get_server_flags(server_flag);
        get_server_flags(slots);
    }

    // detach boiii
    detach_shared_memory(&number, &client_flag, &server_flag, &slots);

    return 0;
}