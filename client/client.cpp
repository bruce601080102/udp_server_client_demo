#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <chrono>
using namespace std;

const char* host = "0.0.0.0";
int port = 7000;
int sock_fd;
struct sockaddr_in serv_name;
int status;
char indata[1024] = {0}, outdata[1024] = {0};

const int max_wait_interval = 10;
const int max_wait_time = 8000;
const int mutiplier = 2;
const int max_retry = 6;
int base = 500;

int main()
{
    // create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // server address
    serv_name.sin_family = AF_INET;
    inet_aton(host, &serv_name.sin_addr);
    serv_name.sin_port = htons(port);

    status = connect(sock_fd, (struct sockaddr *)&serv_name, sizeof(serv_name));
    if (status == -1) {
        perror("Connection error");
        exit(1);
    }

    printf("please input message: ");
    fgets(outdata,1024,stdin);
    printf("send: %s\n", outdata);
    int times = 0;

    while (1) {
        send(sock_fd, outdata, strlen(outdata), 0);
        int nbytes = recv(sock_fd, indata, sizeof(indata), 0);
        if (nbytes <= 0) {
            close(sock_fd);
            printf("server closed connection.\n");
            break;
        }

        printf("recv: %s\n", indata);
        if (times > max_retry-1){
            printf("Max retry reached, exiting program.\n");
            exit(1);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(base)); // 1s
        printf("wait time: %d ms\n", base);
        base*=mutiplier;
        if (base>=max_wait_time){
            base = max_wait_time;
        }

        printf("=====\n");
        times ++;
    }

    return 0;
}