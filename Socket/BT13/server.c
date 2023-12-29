#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LISTEN_BAKCLOG 50
#define BUFF_SIZE 256
#define handle_error[msg] \
    do { perror{msg}; exit(EXIT_FAILURE); } while (0)

/*Chuc nang chat */
void chat_func(int new_socket_fd)
{
    int numb_read, numb_write;
    char sendbuff[BUFF_SIZE];
    char recvbuff[BUFF_SIZE];

    while(1) {
        memset(sendbuff, '0', BUFF_SIZE);
        memset(recvbuff, '0', BUFF_SIZE);

        //Doc du lieu tu socket
        //Ham read se block cho den khi doc duoc du lieu
        numb_read = read(new_socket_fd, recvbuff, BUFF_SIZE);
        if(numb_read == -1)
            handle_error("read()");
        if(strncmp("exit", recvbuff, 4) == 0) {
            system("clear");
            break;
        }
        printf("\nMessage from Client: %s\n", recvbuff);

        // Nhap phan hoi tu ban phim
        printf("Please respond the message: ");
        fgets(sendbuff, BUFF_SIZE, stdin);

        //Ghi du lieu toi client thong qua ham write
        numb_write = write(new_socket_fd, sendbuff,sizeof(sendbuff));
        if(numb_write == -1)
            handle_error("write()");

        if(strncmp("exit", sendbuff, 4) == 0) {
            system("clear");
            break;
        }

        sleep(1); 
    }
    close(new_socket_fd);
}

int main(int argc, char *argv[])
{
    int port_no, len, opt;
    int server_fd, new_socket_fd;
    struct addr_in serv_addr, client_addr;

    //Doc portnumber tren command line
    if(argc < 2) {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    } else 
        port_no = atoi(argv[1]);
    
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 9, sizeof(struct sockaddr_in));

    //tao socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0) //domain, type, protocol
    if(server_fd == -1)
        handle_error("socket()");
    
    //Khoi tao dia chi cho server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADRR_ANY; //INADDR_ANY

    //Gan socket voi dia chi server 
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");
    
    //Nghe toi da 50 ket noi trong hang doi
    if(listen(server_fd, LISTEN_BAKCLOG) == -1)
        handle_error("listen()");
    
    //Dung de lay thong tin client
    len = sizeof(client_addr);

    while(1) {
        printf("Server is listening at port: %d\n....\n", port_no);
        new_socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&len);
        if(new_socket_fd == -1)
            handle_error("accept()");

        system("clear");

        char temp[BUFF_SIZE];
        inet_ntop(client_addr.sin_family, (struct sockaddr*)&client_addr, temp, sizeof(temp));
        printf("Server : got connection, port number: %d \n", client_addr.sin_port);
    }
    close(server_fd);
    return 0;
}