#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void signal_handler(int signum){
    printf("I'm in signal handler\n");
    wait(NULL);
}

int main()
{
    pid_t child_pid = fork();

    if(child_pid == 0) {
        printf("I am Child, myPID: %d\n", getpid());
    }
    else {
        signal(SIGCHLD, signal_handler);
        printf("I am Parent\n");
        while(1);
    }
}