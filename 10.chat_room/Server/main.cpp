/*************************************************************************
	> File Name: main.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 10时39分32秒
 ************************************************************************/

#include "../Common/head.h"
#include "../Common/thread_pool.h"
#include "./server_class.h"
#include "./user_recv.h"
Ser::Server server;
Ser::User user[MAX_USER];
Ser::Pack online_pack;
Ser::All_user all_user;
struct epoll_event rev, revents[MAX_USER];
struct epoll_event ev, events[MAX_USER];

struct Thread_pool t_pool;
int epollfd, repollfd;

void ban_ctlc(int) {
    //定义一个服务端退出包
    Ser::Pack quite_pack;
    //清空退出包
    bzero(&quite_pack, sizeof(quite_pack));
    //标记为退出
    quite_pack.type =  QUITE;
    for (int i = 0; i < MAX_USER; i++) {
        //如果用户不在线走下一个
        if (user[i].get_online() == 0) continue;
        //向所有在线用户发送服务端退出包
        send(user[i].get_fd(), &quite_pack, sizeof(quite_pack), 0);
        //关闭相应的套接字
        close(user[i].get_fd());
    }
    //清空线程池开辟的空间
    thread_pool_clear(&t_pool);
    //退出
    exit(1);
}


int main(int argc, char *argv[]) {
    //接受Ctrl+c信号，调用ban_ctlc函数
    signal(SIGINT, ban_ctlc);
    //定义监听套接字
    int sockfd;
    //定义线程
    pthread_t draw_t, user_t, heart_t;
    //调用函数,初始化套接字
    sockfd = server.Server_socket();
    //初始化epollfd与repollfd
    epollfd = epoll_create(10);
    repollfd = epoll_create(MAX_USER);
    //清空user和all_user
    bzero(&user, sizeof(user));
    bzero(&all_user, sizeof(all_user));
    //all_user初始化 最开始有0个人在线
    all_user.num = 0;
    //初始化线程池
    thread_pool_init(&t_pool);
    //user_t线程执行从反应堆
    pthread_create(&user_t, NULL, user_recv, NULL);
    //心跳线程
    //pthread_create(&heart_t, NULL, heart_t, NULL);
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    //计入死循环
    while(1) {
        //等待IN事件
        int nfds = epoll_wait(epollfd, events, MAX_USER, -1);
        for (int i = 0; i < nfds; i++) {
            Ser::User user;
            if (events[i].data.fd == sockfd) {
                //获取新的套接字
                int new_sockfd = server.Server_accept();
                cout << "new_sockfd == " << new_sockfd << endl;
                if (new_sockfd > 0) {
                    ev.data.fd = new_sockfd;
                    ev.events = EPOLLIN;
                    //向从反应堆添加事件
                    epoll_ctl(repollfd, EPOLL_CTL_ADD, new_sockfd, &ev);
                }
            } 
        }
        cout << "new people come on " << endl;
    }
    return 0;
}
