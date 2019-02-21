/*************************************************************************
	> File Name: Socket.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月21日 星期四 20时33分50秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 10000
#define DEST_IP "192.168.1.40"
#define DEST_PORT 8731

int main() {
    int sockfd;
    struct sockaddr_in dest_addr;
    char buf[MAX] = {"陈常寅"};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    dest_addr.sin_port = htons(DEST_PORT);
    connect(sockfd, (struct sockaddr*)&dest_addr,sizeof(struct sockaddr ));
    while(strcmp(buf,"exit")) {
        send(sockfd, buf, strlen(buf)+1, 0);
        scanf("%s", buf);        
    }
    close(sockfd);
    return 0;
}

