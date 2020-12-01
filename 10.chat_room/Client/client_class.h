/*************************************************************************
	> File Name: client_class.h
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 11时17分34秒
 ************************************************************************/

#ifndef _CLIENT_CLASS_H
#define _CLIENT_CLASS_H
#include "../Common/head.h"

#define MAXBUFF 1024
#define SERVERPORT 8888
#define IP "192.168.2.54"
#define MAX_USER 100

namespace Cli{

struct User{
    int fd;
    int online;
    char name[30];
};

struct All_user{
    int num;
    Cli::User user[MAX_USER];
};

struct Point{
    int x;
    int y;
};

struct Map{
    int width;
    int heigth;
    struct Point start;//起始位置
    int gate_width;
    int gate_heigth;
};

struct Pack {
    int type;
    char name[30];
    char to_name[30];
    char msg[MAX_MSG];
};

class Client{
public:
    Client();
    ~Client();
    int Client_socket();
    void Client_connect();
    void Client_send(Pack *pack);
    void Client_recv();
    void clear_recvbuff();
    char *get_recvbuff();
    int get_sockfd();
    int get_newsockfd();
    int get_ret();
    int get_sendret();
    int get_recvret();
private:
    int sockfd;
    int newsockfd;
    int ret;
    int recv_ret;
    int send_ret;
    int recvret;
    int sendret;
    int on;
    socklen_t serverlen;
    char recv_buff[MAXBUFF];
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
};
}//Cli
#endif
