#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) 
{
    int ret;
    int status;
    pid_t child_pid;

    child_pid = fork();
    if(0 == child_pid) {
        /* Process con */
        printf("\nIm the child process\n");
        printf("My PID is: %d\n", getpid());
        printf("Child process terminate after 5 seconds\n");
        sleep(5);
    }
    else {
        /*Process cha*/
        ret = waitpid(child_pid, &status, WUNTRACED);
        if(ret == -1){
            printf("waitpid() unsuccessful\n");
        }
        printf("\nIm the parent process\n");
        printf("My PID is: %d\n", getpid());
        printf("My PID child process: %d\n", ret);
        
        if(WIFEXITED(status)){
            printf("Normally termination, status = %d\n", WEXITSTATUS(status));
        } 
        else if(WIFSIGNALED(status)){
            printf("Abnormally termination, signal value = %d\n", WTERMSIG(status));
        }
    }

    return 0;
}