/*************************************************************************
	> File Name: main.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 11时15分57秒
 ************************************************************************/

#include "./user_recv.h"
#include "../Common/head.h"
#include "./client_class.h"
#include "./get_conf_value.h"
#include "./user_draw.h"

Cli::Map court; 
Cli::Pack recv_pack;
Cli::Pack send_pack;
int sockfd;


WINDOW *Friend_win, *Chat_win, *Write_win, *Message_win;

pthread_t draw_t, recv_t;
pthread_mutex_t draw_lock;
pthread_mutex_t lock;
map<string, int> online_user;
const char* conf = "./client.conf";
char send_buff[100] = {0};


void ban_ctlc(int ) {
    Cli::Pack quite_pack;
    bzero(&quite_pack, sizeof(quite_pack));
    quite_pack.type = QUITE;
    strcpy(quite_pack.name, send_buff);
    send(sockfd, &quite_pack, sizeof(quite_pack), 0);
    close(sockfd);
    clear();
    endwin();
    cout << "exit" << endl;
    exit(1);
}


int main(int argc, char* argv[]) {
    signal(SIGINT, ban_ctlc);
    Cli::Client client;
    sockfd = client.Client_socket();
    client.Client_connect();
    court.width = atoi(get_conf_value(conf, "COLS"));
    court.heigth = atoi(get_conf_value(conf, "LINES"));
    court.start.x = 1;
    court.start.y = 1;

    setlocale(LC_ALL, "");
    initfootball();
    
    char recv_buff[100] = {0};
    
    strcpy(send_buff, get_conf_value(conf, "NAME"));
    send(sockfd, &send_buff, sizeof(send_buff), 0);
    bzero(&send_pack, sizeof(send_pack));
    bzero(&recv_pack, sizeof(recv_pack));
    Cli::Pack on_pack;
    bzero(&on_pack, sizeof(on_pack));
    recv(sockfd, &on_pack, sizeof(on_pack), 0);
    if (on_pack.type == QUITE) {
        ban_ctlc(1);
    }
    Cli::All_user *all_user = new Cli::All_user;
    
    recv(sockfd, all_user, sizeof(*all_user), 0);
    for (int i = 0; i < all_user->num; i++) {
        if (all_user->user[i].online == 1) online_user.insert(make_pair(all_user->user[i].name, 1));
    }
    
    delete all_user;
    pthread_create(&draw_t, NULL, user_draw, NULL);
    strcpy(send_pack.name, send_buff);
    send_pack.type = PUBLIC_CHAT;
    
    pthread_create(&recv_t, NULL, user_recv, NULL);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    while(1) {
        int c = getchar();
        switch(c) {
            case 13: send_chat(); break;
            default: break;
        }
    }

    return 0;
}
