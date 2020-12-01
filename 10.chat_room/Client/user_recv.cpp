/*************************************************************************
	> File Name: user_recv.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月26日 星期四 10时10分58秒
 ************************************************************************/
#include "./client_class.h"
#include "../Common/head.h"
#include "./user_recv.h"
#include "./user_draw.h"

extern Cli::Pack recv_pack;
extern pthread_mutex_t draw_lock;
extern int sockfd;

void *user_recv(void *argc) {
    while(1) {
        Cli::Pack recvmsg;
        int recv_ret = recv(sockfd, &recvmsg, sizeof(recvmsg), 0);
        if (recv_ret < 0) continue;
        pthread_mutex_lock(&draw_lock);
        show_Chat_win(recvmsg);
        pthread_mutex_unlock(&draw_lock);
    }
}
