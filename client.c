#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char** argv)
{
    char user_input[1000];
    unsigned long int number;
    int n_requests = 0;

    key_t key = ftok("shmfile", 1337);
    int shmid = shmget(key, sizeof(unsigned long int), 0666 | IPC_CREAT);
    unsigned long int* shared_number;

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

        printf("NUMBER: %lu \n", number);
        /* store in shared memory */
        shared_number = (unsigned long int*) shmat(shmid, (void*)0, 0);
        *shared_number = number;

        // detach boiii
        shmdt(shared_number);

    }

    return 0;
}
