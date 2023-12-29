#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{

    pid_t child_pid;

    child_pid = fork();
    if(0 == child_pid) {
        /* Process con */
        printf("\nIm the child process");
        printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid());
        while(1);
    }
    else {
        /*Process cha*/
        printf("\nIm the parent process");
        printf("My PID is: %d\n", getpid());
        while(1);
    }

    return 0;
}