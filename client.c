#include "common.h"



// check the query status and display any new info
void get_queries()
{

}



int main(int argc, char** argv)
{
    char user_input[1000];
    int n_requests = 0;

    uli* number;
    int* client_flag;
    int* server_flag;

    get_shared_memory(&number, &client_flag, &server_flag);

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


        // blocking, will wait for server to read
        while (*client_flag == 1)
        {

        }

        *number =  strtoul(user_input, NULL, 0);
        *client_flag = 1;
        printf("NUMBER: %lu \n", *number);

        // wait for the server to read number
        while (*client_flag == 1)
        {

        }

        // server writes the slot the number is in into the number variable
        used_slot = *number;

    }

    // detach boiii
    detach_shared_memory(&number, &client_flag, &server_flag);

    return 0;
}