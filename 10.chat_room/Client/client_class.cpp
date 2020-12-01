/*************************************************************************
	> File Name: client_class.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 11时17分38秒
 ************************************************************************/
#include "./client_class.h"
#include "../Common/head.h"
#include "./get_conf_value.h"
extern const char* conf;

namespace Cli{
    Client::Client() {
        std::cout << "Client_class start" << std::endl;
        this->serverlen = sizeof(struct sockaddr);
        this->on = 1;
        this->serveraddr.sin_family = AF_INET;
        this->serveraddr.sin_port = htons(atoi(get_conf_value(conf, "SERVERPORT")));
        this->serveraddr.sin_addr.s_addr = inet_addr(get_conf_value(conf, "SERVERIP"));
    }

    Client::~Client() {
        close(this->newsockfd);
        close(this->sockfd);
        cout << "Client_class end" << endl;
    }

    int Client::Client_socket() {
        this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (this->sockfd == -1) {
            cout << "Client_socket ERROR! sockfd == -1 " << endl;
            exit(-1);
        }
       /* 
        this->ret = setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &this->on, sizeof(this->on));
        if (this->ret == -1) {
            cout << "Client_setsockopt ERROR! ret == -1" << endl;
        }*/
        return this->sockfd;
    }

    void Client::Client_connect() {
        this->ret = connect(this->sockfd, (struct sockaddr*)&this->serveraddr, this->serverlen);
        if (this->ret < 0) {
            perror("connect");
            exit(-1);
        }
    }
    
    void Client::Client_send(Pack *pack) {
        send(this->sockfd, &pack, sizeof(pack), 0);
    }

    void Client::Client_recv() {
        this->recv_ret = recv(this->sockfd, this->recv_buff, sizeof(this->recv_buff), 0);
    }

    void Client::clear_recvbuff() {
        bzero(this->recv_buff, sizeof(this->recv_buff));
    }

    int Client::get_recvret() {
        return this->recvret;
    }

    int Client::get_sendret() {
        return this->sendret;
    }

    int Client::get_sockfd(){
        return this->sockfd;
    }

    char *Client::get_recvbuff() {
        return this->recv_buff;
    }

    int Client::get_newsockfd() {
        return this->newsockfd;
    }

    int Client::get_ret() {
        return this->ret;
    }
}

