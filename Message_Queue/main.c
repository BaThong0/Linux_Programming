#include<stdio.h>
#include<string.h>
#include<mqueue.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/msg.h>

#define MQ_MODE (S_IRUSR | S_IWUSR) 
typedef struct sinh_vien {
        char hoten[50];
        int tuoi;
        char quequan[100];
        char sdt[100];
} sinh_vien;
typedef struct message {
    long int id;
    sinh_vien sinhvien;
        
} message;

void Print_Menu() {
    printf("0. Thoat chuong trinh\n");
    printf("1. Nhap thong tin sinh vien\n");
    printf("2. Hien thi thong tin cua sinh vien\n");
    printf("3. Hien thi toan bo danh sach sinh vien\n");
    printf("4. Xoa 1 sinh vien ra khoi danh sach\n");
    printf("5. Moi ban chon so: ");
}
int Nhap_so() {
    int num;
    scanf("%d", &num);
    while(num < 0 || num > 4) {
        printf("So phai tu 0 den 4, xin moi nhap lai: ");
        scanf("%d", &num);
    }
    return num;
}
void Nhap_thong_tin(message *data, int id) {
    mq_receive(id, (char *)&data, sizeof(data), NULL);
    printf("Nhap id: "); 
    scanf("%ld", &data->id);
    while(data->id < 0) {
        printf("Invalid ID, try again: ");
        scanf("%li", &data->id);
    }
    printf("Nhap ho ten: ");
    scanf("%9s", &data->sinhvien.hoten);
    printf("Nhap tuoi: ");
    scanf("%d", &data->sinhvien.tuoi);
    printf("Nhap que quan: ");
    scanf("%9s", &data->sinhvien.quequan);
    printf("Nhap sdt: ");
    scanf("%9s", &data->sinhvien.sdt);
    mq_send(id, (char *)&data, sizeof(struct message), 0);
}
void Hienthi_thongtin(message *data, long int id, int mid) {
    mq_receive(id, (char *)&data, sizeof(data), NULL);
    printf("Hien thi tat ca sinh vien co id%li\n", id);
    if(id == data->id) {
        printf("Ten sinh vien la: %s\n", data->sinhvien.hoten);
        printf("Tuoi cua sinh vien la: %d\n", data->sinhvien.tuoi);
        printf("SDT cua sinh vien la: %s\n", data->sinhvien.sdt);
        printf("Que quan cua sinh vien la: %s\n", data->sinhvien.quequan);
    } else {
        printf("Khong trung khop!\n");
    } 
    mq_send(mid, (char *)&data, sizeof(data), NULL);
}
void Hienthi_tatca() {
    printf("Hien thi het\n");
}

void xoaSinhvien(message *data, long int id, int mid) {
    printf("Xoa sinh vien\n");
    if(id == data->id) {
        printf("Trung khop! Bat dau xoa ...\n");
        mq_receive(mid, (char *)&data, sizeof(data), NULL);
    } else {
        printf("Khong tim thay sinh vien co ID nhu mong muon\n");
    }
}
int main(int argc, char *argv[]) {

    //attr.mq_maxmsg = 5;
    //attr.mq_msgsize = 2048;

    printf("Create mqueu\n");
    mqd_t mqid = mq_open("/mqueue", O_RDWR | O_CREAT | O_NONBLOCK, MQ_MODE, NULL);
    if(mqid == -1) {
        printf("mq_open() error %d: %s\n", errno, strerror(errno));
        return -2;
    }

    // if(mq_getattr(mqid, &attrp) != 0) {
    //     printf("mq_open() error %d: %s\n", errno, strerrno(errno));
    //     return -3;
    // }

    // if(attrp.mq_flags == 0) 
    //     printf("mq_flags = 0\n");
    // else 
    //     printf("mq_flags = O_NONBLOCK\n");
    
    // printf("mq_maxmsg = %ld,\n", attrp.mq_maxmsg);
    // printf("mq_msgsize = %ld\n", attrp.mq_msgsize);
    // printf("mq_curmsgs = %ld\n", attrp.mq_curmsgs);
    message Mes;
    int num;
    long id;
    while(1) {
        printf("Hay chon hanh dong ban muon lam, nhan so tuong ung!\n");
        Print_Menu();
        num = Nhap_so();
        switch (num)
        {
        case 0: 
            printf("Thoat chuong trinh!\n");
            exit(EXIT_SUCCESS);
        case 1:
            Nhap_thong_tin(&Mes, mqid);
            break;
        case 2:
            printf("Nhap ID can tim: ");
            scanf("%li", &id);
            Hienthi_thongtin(&Mes, id, mqid);
            mq_send(mqid, (char *)&Mes, sizeof(Mes), NULL);
            id = 0;
            break;
        case 3:
            Hienthi_tatca();
        case 4:
            printf("Nhap ID can xoa: ");
            scanf("%li", &id);
            xoaSinhvien(&Mes, id, mqid);
            id = 0;
            break;
        default:
            break;
        }
    }
    mq_unlink("/mqueue");
    return 0;   
}