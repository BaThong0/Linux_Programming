#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void sig_handler(int signum)
{
    printf("I'm signal handler\n");
    wait(EXIT_SUCCESS);
}

int main()
{
    sigset_t new_set, old_set;
    if(signal(SIGINT, sig_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    
    sigemptyset(&new_set);
    sigemptyset(&old_set);

    sigaddset(&new_set, SIGINT);
    printf("New set is %x\n", new_set);

    if(sigprocmask(SIG_SETMASK, &new_set, &old_set) == 0) {
        if(sigismember(&new_set, SIGINT) == 1) {
            printf("SIGINT exist\n");
        }
        else if(sigismember(&new_set, SIGINT) == 0) {
            printf("SIGINT not exist\n");
        }
    }
    while(1);

    return 0;
}