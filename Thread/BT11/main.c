#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_id1, thread_id2;

typedef struct {
    char HoTen[30];
    int namsinh;
    char SDT[10];
    char QueQuan[30];
} human;

static void *thr_handle(void *args) {
    pthread_t tid = pthread_self();
    human *data = (human *) args;

    if(pthread_equal(tid, thread_id1)) {
        printf("Hi I'm thread_id1\n");
    }
    else if(pthread_equal(tid, thread_id2)) {
        printf("Hi I'm thread_id2\n");
        printf("HoTen: %s\nNamsinh: %d\nSDT: %s\nQueQuan: %s\n", data->HoTen, data->namsinh, data->SDT, data->QueQuan);
    }
}

int main(int argc, char const *argv[]) {
    int ret;
    human data = { "Ba Thong", 2002, "012345678", "HaNoi"};
    
    if(ret = pthread_create(&thread_id1, NULL, &thr_handle, NULL)) {
        printf("pthread_create() error number = %d\n", ret);
        return -1;
    }
    sleep(2);

    if(ret = pthread_create(&thread_id2, NULL, &thr_handle, &data)) {
        printf("pthread_create() error number = %d\n", ret);
        return -1;
    } 
    sleep(5);

    return 0;
}