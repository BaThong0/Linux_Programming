//C program to design a shell in Linux 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported 

//Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

//Greeting shell during startup
void init_shell() {
    clear();
    printf("\n\n\n\n******************"
        "************************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

//Function to take input 
int takeInput(char* str) {
    char* buf;

    buf = readline("\n>>> ");
    if(strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

//Function to print Current Directory
void printDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

//Function where the system command is executed 
void execArgs(char** parsed) {
    //Forking a child 
    pid_t pid = fork();

    if(pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if(pid == 0) {
        if(execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        //waiting for child to terminate 
        wait(NULL);
        return;
    }
}

//Function where the piped system commands is executed 
void execArgsPiped(char** parsed, char** parsedpipe) {
    //0 is read, 1 is write 
    int pipefd[2];
    pid_t p1, p2;

    if(pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if(p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if(p1 == 0) {
        //Child 1 executing..
        //It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        // close(pipefd[1]);

        if(execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    }
    close(pipefd[1]);
    //Child 2 executing
    p2 = fork();

    if(p2 < 0) {
        printf("\nCould not fork");
        return;
    }

    //It only needs to read at the read end
    if(p2 == 0) {
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    // close(pipefd[0]);
        if(execvp(parsedpipe[0], parsedpipe) < 0) {
            printf("\nCould not execute command 2..");
            exit(0);
        }
    }
    close(pipefd[0]);
    //Parent executing, waiting for two children
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
}

//Function where the built_piped commands is executed 
void execArgsBuiltPiped(char** parsed, char** parsedpipe) {
    //0 is read, 1 is write 
    int pipefd[2];
    pid_t p1, p2;

    if(pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if(p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if(p1 == 0) {
        //Child 1 executing..
        //It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        // close(pipefd[1]);

        if(execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    }
    close(pipefd[1]);
    //Child 2 executing
    p2 = fork();

    if(p2 < 0) {
        printf("\nCould not fork");
        return;
    }

    //It only needs to read at the read end
    if(p2 == 0) {
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    // close(pipefd[0]);
        if(execvp(parsedpipe[0], parsedpipe) < 0) {
            printf("\nCould not execute command 2..");
            exit(0);
        }

        if(execvp(parsedpipe[1], parsedpipe) < 0) {
            printf("\nCould not execute command 2..");
            exit(0);
        }
    }
    close(pipefd[0]);
    //Parent executing, waiting for two children
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
}


//Function where the redirected output system command is executed
void execArgsDirOut(char** parsed, char**parsedpipe) {
    //Create a child process
    pid_t pid = fork();

    if(pid == -1) {
        //Fork failed
        _exit(1);
    } else if(pid == 0) {
        //Child process
        //Redirected stdout to a file 
        int fd = open(parsedpipe[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if(fd == -1) {
            _exit(1); //Error opening the file
        }

        //Redirect stdout to the file descriptor
        dup2(fd, STDOUT_FILENO);

        //Close the file descriptor
        close(fd);

        //Execute the parsed command
        if(execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 2..");
            exit(0);
        }
        
        //If execvp fails
        _exit(1);
    } else {
        //Parent process
        //Wait for the child process to complete 
        waitpid(pid, NULL, 0);
    }
}

//Function where the redirected input system command is executed
void execArgsDirIn(char** parsed, char**parsedpipe) {
    //Create a child process
    pid_t pid = fork();

    if(pid == -1) {
        //Fork failed
        _exit(1);
    } else if(pid == 0) {
        //Child process
        //Redirected stdout to a file 
        int fd = open(parsedpipe[0], O_RDONLY);
        if(fd == -1) {
            _exit(1); //Error opening the file
        }

        //Redirect stdout to the file descriptor
        dup2(fd, STDIN_FILENO);

        //Close the file descriptor
        close(fd);

        //Execute the parsed command
        if(execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 2..");
            exit(0);
        }
        
        //If execvp fails
        _exit(1);
    } else {
        //Parent process
        //Wait for the child process to complete 
        waitpid(pid, NULL, 0);
    }
}
//Help command builtin
void openHelp() {
    puts("\n***WELCOME TO MY SHELL HELP***"
         "\nCopyright @ Suprotik Dey"
         "\n-Use the shell at your own risk..."
         "\nList of Commands supported:"
         "\n>cd"
         "\n>ls"
         "\n>exit"
         "\n>all other general commands available in UNIX shell"
         "\npipe handling"
         "\n>improper space handling");
    return;
}

//Function to execute builtin commands
int ownCmdHandler(char** parsed) {
    int NoOfOwnCmds = 4, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";

    for(i = 0; i < NoOfOwnCmds; ++i) {
        if(strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
        case 1: 
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
            openHelp();
            return 1;
        case 4:
            username = getenv("USER");
            printf("\nHello %s.\nMind that this is "
                "not a place to play around."
                "\nUse help to know more..\n", username);
            return 1;
        default: 
            break;
    }

    return 0;
}

//function for finding pipe 
int parsePipe(char* str, char** strpiped) {
    int i;
    for(i = 0; i < 2; ++i) {
        strpiped[i] = strsep(&str, "|");
        if(strpiped[i] == NULL) {
            break;
        }
    }
    if(strpiped[1] == NULL) {
        return 0; //returns 0 if no pipe is found
    } else {
        return 1;
    }
}

//function for finding built_pipe 
int parseBuilt_Pipe(char* str, char** strpiped) {
    int i;
    for(i = 0; i < 2; ++i) {
        strpiped[i] = strsep(&str, "&");
        if(strpiped[i] == NULL)
            break;
    }
    if(strpiped[1] == NULL) 
        return 0; //returns 0 if no pipe is found
    else {
        return 1;
    }
}

//function for finding redirecting output 
int parseDirect_Out(char* str, char** strdirect_out) {
    int i;
    for(i = 0; i < 2; ++i) {
        strdirect_out[i] = strsep(&str, ">");
        if(strdirect_out[i] == NULL)
            break;
    }
    if(strdirect_out[1] == NULL) 
        return 0; //returns 0 if no redirecting output is found
    else {
        return 1;
    }
}

//function for finding redirecting input 
int parseDirect_In(char* str, char** strdirect_in) {
    int i;
    for(i = 0; i < 2; ++i) {
        strdirect_in[i] = strsep(&str, "<");
        if(strdirect_in[i] == NULL)
            break;
    }
    if(strdirect_in[1] == NULL) 
        return 0; //returns 0 if no redirecting output is found
    else {
        return 1;
    }
}

//function for parsing command words
void parseSpace(char* str, char** parsed) {
    int i;

    for(i = 0; i < MAXLIST; ++i) {
        parsed[i] = strsep(&str, " ");

        if(parsed[i] == NULL) 
            break;
        if(strlen(parsed[i]) == 0)
            --i;
    }
}

int processString(char* str, char** parsed, char** parsedpipe) {
    char* strpiped[2];
    char* strdirect_out[2];
    char* strdirect_in[2];
    char* strbuilt_piped[2];
    int piped = 0;
    int dir_out = 0;
    int dir_in = 0;
    int built_piped = 0;

    piped = parsePipe(str, strpiped);
    dir_out = parseDirect_Out(str, strdirect_out);
    dir_in = parseDirect_In(str, strdirect_in);
    built_piped = parseBuilt_Pipe(str, strbuilt_piped);
    if(piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    } else if(dir_out) {
        parseSpace(strdirect_out[0], parsed);
        parseSpace(strdirect_out[1], parsedpipe);
    } else if(dir_in) {
        parseSpace(strdirect_in[0], parsed);
        parseSpace(strdirect_in[1], parsedpipe);
    } else if(built_piped) {
        parseSpace(strbuilt_piped[0], parsed);
        parseSpace(strbuilt_piped[1], parsedpipe);
    } else {
        parseSpace(str, parsed);
    }
    
    if(ownCmdHandler(parsed))
        return 0;
    else if(dir_out)
        return 3;
    else if(dir_in) 
        return 4;
    else if(built_piped)
        return 5;
    else 
        return 1 + piped;
    
}

int main() {
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while(1) {
        //print shell line 
        printDir();
        //take input 
        if(takeInput(inputString))
            continue;
        //process
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
        //execflag returns 0 if there is no command
        //or it is a builtin command,
        //1 if it is a simple command
        //2 if it is including a pipe.
        //3 if it is including redirected output
        //4 if it is including redirected input
        //5 if it is including a built-in pipe
        //execute
        if(execFlag == 1) 
            execArgs(parsedArgs);
        
        else if(execFlag == 2) 
            execArgsPiped(parsedArgs, parsedArgsPiped);

        else if(execFlag == 3)
            execArgsDirOut(parsedArgs, parsedArgsPiped);
        
        else if(execFlag == 4) 
            execArgsDirIn(parsedArgs, parsedArgsPiped);
        
        else if(execFlag == 5) 
            execArgsBuiltPiped(parsedArgs, parsedArgsPiped);
        
        else 
            printf("Not having this command, maybe wrong syntax, try again!");
    }
    return 0;
}