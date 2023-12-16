#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_id1, thread_id2, thread_id3;

typedef struct {
    char HoTen[30];
    char DOB[10];
    char QueQuan[30];
} pthread_data;

pthread_data data;

const char* file_path = "thongtinsinhvien.txt";

static void *thr_handle1(void *args) {
    printf("I'm thread 1 handler\n");

    pthread_mutex_lock(&lock1);
    pthread_data *info = (pthread_data*) args;
    printf("Nhap HoTen: ");
    scanf("%s", info->HoTen);
    printf("Nhap DOB: ");
    scanf("%s", info->DOB);
    printf("Nhap que quan: ");
    scanf("%s", info->QueQuan);
    pthread_mutex_unlock(&lock1);

    pthread_exit(NULL);
}

static void *thr_handle2(void *args) {
    printf("I'm thread 2 handler\n");

    pthread_mutex_lock(&lock2);
    pthread_data *info = (pthread_data*) args;
    int fd = open(file_path, O_APPEND | O_CREAT | O_WRONLY, 0777);
    if(fd == -1) {
        perror("open");
        exit(1);
    }
    char buffer[100];
    sprintf(buffer, "%s%s%s\n", info->HoTen, info->DOB, info->QueQuan);
    write(fd, buffer, strlen(buffer));
    close(fd);
    pthread_mutex_unlock(&lock2);

    pthread_exit(NULL);
}

static void *thr_handle3(void *args) {
    printf("I'm thread 3 handler\n");

    pthread_mutex_lock(&lock3);
    pthread_data *info = (pthread_data*) args;
    char* read_data = (char*)calloc(100, sizeof(char));
    int fd = open(file_path, O_APPEND | O_CREAT | O_RDONLY, 0777);
    if(fd == -1) {
        perror("open");
        exit(1);
    }
    
    char buffer[100];
    int bytes_read;
    while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    // read_data[sz] = '\0';
    // printf("%s\n", read_data);
    close(fd);
    pthread_mutex_unlock(&lock3);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    int ret;

    if(ret = pthread_create(&thread_id1, NULL, &thr_handle1, &data)) {
        printf("pthread_create() error number = %d\n", ret);
        return -1;
    }
    pthread_join(thread_id1, NULL);
    if(ret = pthread_create(&thread_id2, NULL, &thr_handle2, &data)) {
        printf("pthread_create() error number = %d\n", ret);
        return -1;
    } 
    

    pthread_join(thread_id2, NULL);
    if(ret = pthread_create(&thread_id3, NULL, &thr_handle3, &data)) {
        printf("pthread_create() error number = %d\n", ret);
        return -1;
    } 
    
    
    while(1) {
        pthread_join(thread_id3, NULL);
        if(ret = pthread_create(&thread_id1, NULL, &thr_handle1, &data)) {
            printf("pthread_create() error number = %d\n", ret);
            return -1;
        }
        
        
        pthread_join(thread_id1, NULL);
        if(ret = pthread_create(&thread_id2, NULL, &thr_handle2, &data)) {
            printf("pthread_create() error number = %d\n", ret);
            return -1;
        } 
        

        pthread_join(thread_id2, NULL);
        if(ret = pthread_create(&thread_id3, NULL, &thr_handle3, &data)) {
            printf("pthread_create() error number = %d\n", ret);
            return -1;
        }
    }

    return 0;
}