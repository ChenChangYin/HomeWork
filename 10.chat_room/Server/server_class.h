/*************************************************************************
	> File Name: server_class.h
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 10时45分27秒
 ************************************************************************/

#ifndef _SERVER_CLASS_H
#define _SERVER_CLASS_H
#include "../Common/head.h"
#define SERVER_PORT 8888
#define MAXBUFF 2048
#define MAX_USER 100
#define TRUE 1
#define FALSE 0
namespace Ser{
class Server{ 
public:
    Server();
    ~Server();
    int Server_socket();
    int Server_accept();
    void Server_send(char *send_buff);
    void Server_recv();
    char *get_recvbuff();
    void clear_recvbuff();
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
    int sendret;
    int listen_num;
    int on;
    socklen_t serverlen;
    char recv_buff[MAXBUFF];
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
};

class User{
public:
    User();
    ~User();
    void chmod_online(int val);
    void chmod_name(char *name);
    void chmod_fd(int sockfd);
    int get_fd();
    int get_online();
    char* get_name();
private:
    int fd;
    int online;
    char name[30];
};

struct Pack{
    int type;
    char name[30];
    char to_name[30];
    char msg[1024];
};

struct All_user{
    int num;
    Ser::User user[MAX_USER];
};
}//Ser

#endif
