/*************************************************************************
	> File Name: server_class.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 10时46分28秒
 ************************************************************************/
#include "./server_class.h"
#include "../Common/head.h"

extern Ser::User user[MAX_USER];
extern Ser::Pack online_pack;
extern Ser::All_user all_user;
namespace Ser{
    Server::Server() {
        cout << "Server_class start" << endl;
        this->on = 1;
        this->listen_num = 20;
        this->serverlen = sizeof(struct sockaddr);
        this->serveraddr.sin_family = AF_INET;
        this->serveraddr.sin_port = htons(SERVER_PORT);
        this->serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    Server::~Server() {
        close(this->newsockfd);
        close(this->sockfd);
        cout << "Server_class end" << endl;
    }

    int Server::Server_socket() {
        this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (this->sockfd == -1) {
            cout << "Server_socket ERROR! sockfd == -1 " << endl;
            exit(-1);
        }

        this->ret = setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &this->on, sizeof(this->on));
        if (this->ret == -1) {
            cout << "Server_setsockopt ERROR! ret == -1" << endl;
        }

        this->ret = bind(this->sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr_in));
        if (this->ret == -1) {
            cout << "Server_bind ERROR! ret == -1" << endl;
            exit(-1);
        }
        cout << "Server_bind ........" << endl;

        this->ret = listen(this->sockfd, this->listen_num);
        if (this->ret == -1) {
            cout << "Server_listen ERROR! ret == -1" << endl;
            exit(-1);
        }
        cout << "Server_listen ........" << endl;
        return this->sockfd;
    }

    int Server::Server_accept() {
        this->newsockfd = accept(this->sockfd, (struct sockaddr*)&this->clientaddr, &this->serverlen);
        if (this->newsockfd < 0) {
            perror("accept");
            exit(-1);
        }
        char username[100];
        recv(this->newsockfd, username, sizeof(username), 0);
        bzero(&online_pack, sizeof(online_pack));
        online_pack.type = ONLINE;
        strcpy(online_pack.name, username);
        sprintf(online_pack.msg, "%s 加入聊天室", username);
        for (int i = 0; i < MAX_USER; i++) {
            if (user[i].get_online()) {
                if(strcmp(user[i].get_name(), username) == 0) {
                    cout << "重复用户名  --"  << this->newsockfd << endl;
                    Ser::Pack exit_pack;
                    exit_pack.type = QUITE;
                    send(this->newsockfd, &exit_pack, sizeof(exit_pack), 0);
                    close(this->newsockfd);
                    return -1;
                }
                send(user[i].get_fd(), &online_pack, sizeof(online_pack), 0);
            }
        }
        send(this->newsockfd, &online_pack, sizeof(online_pack), 0);
        for (int i = 0; i < MAX_USER; i++) {
            if (user[i].get_online() == 0) {
                user[i].chmod_name(username);
                user[i].chmod_online(TRUE);
                user[i].chmod_fd(this->newsockfd);
                cout << "all_user : " << all_user.user[all_user.num].get_name() << endl;
                all_user.num++;
                all_user.user[i].chmod_name(username);
                all_user.user[i].chmod_online(TRUE);
                all_user.user[i].chmod_fd(this->newsockfd);
                send(user[i].get_fd(), &all_user, sizeof(all_user), 0);
                break;
            }
        }
        return this->newsockfd;
    }

    void Server::Server_recv() {
        this->recv_ret = recv(this->newsockfd, &this->recv_buff, sizeof(this->recv_buff), 0);
    }

    void Server::Server_send(char* send_buff) {
        this->send_ret = send(this->newsockfd, send_buff, sizeof(send_buff), 0);
    }

    char *Server::get_recvbuff() {
        return this->recv_buff;
    }

    void Server::clear_recvbuff() {
        bzero(this->recv_buff, sizeof(recv_buff));
    }

    int Server::get_recvret() {
        return this->recv_ret;
    }

    int Server::get_sendret() {
        return this->sendret;
    }

    int Server::get_sockfd() {
        return this->sockfd;
    }

    int Server::get_newsockfd() {
        return this->newsockfd;
    }

    int Server::get_ret() {
        return this->ret;
    }
}

namespace Ser{
    User::User(){
        bzero(this->name, sizeof(this->name));
        this->online = FALSE;
        this->fd = -1;
    }
    User::~User() {
    }
    void User::chmod_name(char *name) {
        strcpy(this->name, name);
    }

    void User::chmod_online(int val) {
        this->online = val;
        cout << "online : " << this->online << "val : " << val << endl;
    }

    void User::chmod_fd(int sockfd) {
        this->fd = sockfd;
    }
    
    int User::get_fd() {
        return this->fd;
    }

    int User::get_online(){
        return this->online;
    }

    char* User::get_name(){
        return this->name;
    }
}

