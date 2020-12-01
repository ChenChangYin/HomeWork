/*************************************************************************
	> File Name: send_pack.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月26日 星期四 17时34分46秒
 ************************************************************************/
#include "./client_class.h"
#include "../Common/head.h"
#include "./user_draw.h"
extern Cli::Pack pack;
extern int sockfd;
extern WINDOW *Write_win, *Friend_win;
extern pthread_mutex_t draw_lock;


void send_chat() {
    echo();
    nocbreak();
    bzero(&pack, sizeof(pack));
    pthread_mutex_lock(&draw_lock);
    char temp_msg[128] = {0};
    mvwscanw(Write_win, 2, 2, "%[^\n]s", temp_msg);
    pthread_mutex_unlock(&draw_lock);
    pack.type = PUBLIC_CHAT;
    int len = strlen(temp_msg);
    if (temp_msg[0] == '@') {
        char temp_name[20] = {0};
        int j = 0;
        for (int i = 1; i < len; i++) {
            if (temp_msg[i] != ':') {
                temp_name[j++] = temp_msg[i];
            } else {
                strcpy(pack.to_name, temp_name);
                for (int k = j + 2, l = 0; l < len; k++) {
                    if (k < len) pack.msg[l++] = temp_msg[k];
                    else pack.msg[l++] = '\0';
                }
                break;
            }
        }
        if (strlen(pack.msg)) {
            pack.type = PRIVATE_CHAT;
        }
    } else strcpy(pack.msg, temp_msg);
    if (strlen(pack.msg)) {
        w_gotoxy_puts(Friend_win, 1, 4, pack.to_name);
        w_gotoxy_puts(Friend_win, 1, 5, "csac");
        //send(sockfd, &pack, sizeof(pack), 0);
    }
    cbreak();
    noecho();
    return ;
}
