#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    char user_input[1000];
    unsigned long int number;
    int n_requests = 0;

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

        n_requests ++;

        /* convert to number */
        number = strtoul(user_input, NULL, 0);

        printf("NUMBER: %ul \n", number);
        /* store in shared memory */


    }

    return 0;
}
