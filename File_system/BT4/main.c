#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FILENAME "Test.txt"
int main() {   
    int fd, wr;
    const char *data="APPENDING ";

    //Open file 
    fd = open(FILENAME, O_RDWR | O_APPEND);
    if(fd==-1) {
        perror("Open");
        exit(1);
    }

    //Seek to the beginning of the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to the beginning of the file");
        close(fd);
        exit(1);
    }

    //Write to file 
    wr = write(fd, data, strlen(data));
    if(wr==-1) { 
        perror("Error writing file ");
        close(fd);
        exit(EXIT_FAILURE);
        
    }

    //Close file 
    close (fd);

    /*
    With O_APPEND flag, pointer will be at the end of file even using lseek function
    */
}