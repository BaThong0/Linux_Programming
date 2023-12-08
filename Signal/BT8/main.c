#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sig_handler1(int signum)
{
    printf("\nIm SIGINT: %d", signum);
}

void sig_handler2(int signum)
{
    if(signum == SIGUSR1) {
        printf("Caught SIGUSR1 signal\n");
    }
}

void sig_handler3(int signum)
{
    if(signum == SIGUSR2) {
        printf("Caught SIGUSR2 signal\n");
    }
}

int main()
{
    if(signal(SIGINT, sig_handler1) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    sleep(4);
    kill(getpid(), SIGINT);

    if(signal(SIGUSR1, sig_handler2) == SIG_ERR){
        printf("Error when register action for SIGUSR1");
        exit(1);
    }
    sleep(4);
    kill(getpid(), SIGUSR1);

    if(signal(SIGUSR2, sig_handler3) == SIG_ERR){
        printf("Error when register action for SIGUSR2");
        exit(1);
    }
    sleep(4);
    kill(getpid(), SIGUSR2);
    while(1) {
        sleep(2);
    }
}