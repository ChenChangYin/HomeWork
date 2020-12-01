/*************************************************************************
	> File Name: user_recv.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月25日 星期三 10时57分53秒
 ************************************************************************/
#include "user_recv.h"
#include "../Common/head.h"
#include "./server_class.h"
#include "../Common/thread_pool.h"

extern int repollfd;
extern Ser::User user[MAX_USER];
extern struct Thread_pool t_pool;
extern struct epoll_event rev, revents[MAX_USER];
extern struct Ser::All_user all_user;

void *recv_run(void *argc) {
    Ser::Pack pack;
    int *recv_fd = (int *)argc;
    int ret;
    //循环接收recv_fd的消息
    while((ret = recv(*recv_fd, &pack, sizeof(pack), 0)) > 0) {
        cout << "pack.type == " << pack.type << endl;
        cout << pack.name << ":" << pack.to_name << "-" << pack.msg << endl;
        //判断消息类别是公聊、私聊、或者退出消息
        if (pack.type == PUBLIC_CHAT) {
            for (int i = 0; i < MAX_USER; i++) {
                //如果是公聊向所有在线用户发送消息
                if (user[i].get_online()) {
                    cout << user[i].get_name() << " i == " << i << endl;
                    send(user[i].get_fd(), &pack, sizeof(pack), 0);
                }
            }
        } else if (pack.type == PRIVATE_CHAT){
            for (int i = 0; i < MAX_USER; i++) {
                //如果是私聊只向私聊人发送消息
                if (strcmp(user[i].get_name(), pack.to_name) == 0){
                    send(user[i].get_fd(), &pack, sizeof(pack), 0);
                    break;
                }
            }

        } else if (pack.type == QUITE) {
            pack.type = OFLINE;
            cout << pack.name << " is quit!" << endl;
            sprintf(pack.msg, "%s 退出聊天室", pack.name);
            for (int i = 0; i < MAX_USER; i++) {
                //通知所有在线人员***退出聊天室
                if (user[i].get_online()) {
                    if (strcmp(user[i].get_name(), pack.name) == 0) {
                        //置为离线状态
                        user[i].chmod_online(0);
                        //关闭对应套接字
                        close(user[i].get_fd());
                        //在线人数减1
                        all_user.num--;
                        //将此人的在线信息置为离线
                        all_user.user[i].chmod_online(0);
                        continue;
                    } else {
                        send(user[i].get_fd(), &pack, sizeof(pack), MSG_NOSIGNAL);
                    }
                }
            }
            //从repollfd从反应堆进中移除
            epoll_ctl(repollfd, EPOLL_CTL_DEL, *recv_fd, &rev);
        }
    }
}


void *user_recv(void *arg){
    int nfds;
    //从反应堆进入死循环
    while(1) {
        //等待事件进入
        nfds = epoll_wait(repollfd, revents, MAX_USER, -1);
        cout << "user_recv nfds == " << nfds << endl;
        if (nfds == -1) {
            perror("epoll_wait");
            exit(-1);
        }
        for (int i = 0; i < nfds; i++) {
            //找到对应的事件
            if (revents[i].events & EPOLLIN) {
                //调用线程池 寻找线程运行recv_run函数
                add_task(&t_pool, recv_run, (void*)&revents[i].data.fd);
            }
        }
    }
    return NULL;
}
