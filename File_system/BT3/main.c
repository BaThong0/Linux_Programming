#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>

#define FILENAME "Test.txt"
int main() {
    struct stat sb;
    int fd, wr;
    const char* data = "Hello, Thong\n";
    //Create or open the file for writing 
    fd = open(FILENAME, O_CREAT | O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    //Write data to the file 
    wr = write(fd, data, strlen(data));
    if(wr == -1){
        perror("write");
        close(fd);
        exit(1);
    }
    //Print type of file
    printf("File type:                ");

           switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }

    //Print file name
    printf("File Name:                %s\n", FILENAME);

    //Print file size 
    printf("File size:                %ld bytes\n",  sb.st_size);

    //Print the last time file modification
     printf("Last file modification:   %s", ctime(&sb.st_mtime));

    //Close the file
    close(fd);

    return 0;
}
