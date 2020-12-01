/*************************************************************************
	> File Name: user_draw.cpp
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月26日 星期四 17时49分45秒
 ************************************************************************/
#include "./user_draw.h"
#include "../Common/head.h"
#include "./client_class.h"


extern WINDOW *Chat_win, *Friend_win, *Write_win, *Message_win;
extern Cli::Map court;
extern pthread_mutex_t lock;
extern pthread_mutex_t draw_lock;
extern pthread_t draw_t;
extern Cli::Pack send_pack;
extern int sockfd;
extern map<string, int> online_user;
int message_num = 0, line = 0;


//创建从startx、starty开始的窗口 heigth行 width列窗口
WINDOW *create_newwin(int width, int heigth, int startx, int starty) {
    WINDOW *win;
    win = newwin(heigth, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void destroy_win(WINDOW *win) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

void gotoxy(int x, int y) {
    move(y, x);
}

void gotoxy_putc(int x, int y, int c) {
    move(y, x);
    addch(c);
    move(LINES - 1, 1);
    refresh();
}  

void gotoxy_puts(int x, int y, char* s) {
    move(y, x);
    addstr(s);
    move(LINES - 1, 1);
    refresh();
}  

void w_gotoxy_putc(WINDOW *win, int x, int y, char c) {
    mvwaddch(win, y, x, c);
    move(LINES - 1, 1);
    wrefresh(win);
} 

void w_gotoxy_puts(WINDOW *win, int x, int y, const char *s) {
    mvwprintw(win, y, x, s);
    move(LINES - 1, 1);
    wrefresh(win);
}

void Server_Quit() {
    close(sockfd);
    clear();
    endwin();
    exit(1);
}

void initfootball() {
    //对curses函数库初始化
    initscr();
    //清空屏幕
    clear();
    //检测终端是否支持颜色 之后对curses颜色礼成初始化
    if (!has_colors() || start_color() == ERR) {
        endwin();
        if (start_color() == ERR) {
            cout << "start_color == ERR !" << endl;
        }
        if (!has_colors()) {
            cout << "!has_colors()" << endl;
        }
        fprintf(stderr, "终端不支持颜色！\n");
        exit(1);
    }
    //初始化颜色
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    
    init_pair(8, COLOR_BLACK, COLOR_GREEN);
    init_pair(9, COLOR_BLACK, COLOR_RED);
    init_pair(10, COLOR_BLACK, COLOR_BLUE);
    init_pair(11, COLOR_BLACK, COLOR_YELLOW);
    init_pair(12, COLOR_BLACK, COLOR_CYAN);
    //聊天室窗口
    Message_win = create_newwin(court.width, court.heigth, court.start.x, court.start.y);
    wattron(Message_win, COLOR_PAIR(5));
    box(Message_win, 0, 0);
    wrefresh(Message_win);

    Chat_win = subwin(Message_win, court.heigth - 2, court.width - 2, court.start.y + 1, court.start.x + 1);
    scrollok(Chat_win, 1);

    //好友列表窗口
    Friend_win = create_newwin(19, court.heigth, court.start.x + court.width + 1, court.start.y);
    wattron(Friend_win, COLOR_PAIR(5));
    box(Friend_win, 0, 0);
    wrefresh(Friend_win);

    //输入消息的窗口
    Write_win = create_newwin(court.width + 20, 5, court.start.x, court.start.y + court.heigth + 1);
    wattron(Write_win, COLOR_PAIR(5));
    box(Write_win, 0, 0);
    wrefresh(Write_win);
}

void show_Chat_win(Cli::Pack pack) {
    time_t time_now = time(NULL);
    struct tm *tm = localtime(&time_now);
    char timestr[20] = {0};
    char username[80];
    sprintf(timestr, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
    strcpy(username, "System notification : ");
    if (pack.type == WALL) {
        wattron(Chat_win, COLOR_PAIR(2));
    } else if (pack.type == QUITE) {
        Server_Quit();
    } else if (pack.type == ONLINE) {
        wattron(Chat_win, COLOR_PAIR(2));
        pthread_mutex_lock(&lock);
        online_user.insert(make_pair(pack.name, 1));
        pthread_mutex_unlock(&lock);
        pthread_create(&draw_t, NULL, user_draw, NULL);
    } else if (pack.type == OFLINE) {
        wattron(Chat_win, COLOR_PAIR(2));
        pthread_mutex_lock(&lock);
        online_user.erase(pack.name);
        pthread_mutex_unlock(&lock);
        pthread_create(&draw_t, NULL, user_draw, NULL);
    } else if (pack.type == PRIVATE_CHAT){
        wattron(Chat_win, COLOR_PAIR(2));
        sprintf(username, "%s PRIVATE CHAT:", pack.name);
    } else {
        wattron(Chat_win, COLOR_PAIR(7));
        sprintf(username, "%s : ", pack.name);
    }
    if (message_num < 23) {
        w_gotoxy_puts(Chat_win, 10, message_num, username);
        wattron(Chat_win, COLOR_PAIR(3));
        w_gotoxy_puts(Chat_win, 10 + strlen(username), message_num, pack.msg);
        wattron(Chat_win, COLOR_PAIR(4));
        w_gotoxy_puts(Chat_win, 1, message_num, timestr);
        message_num++;
    } else {
        message_num = 22;
        scroll(Chat_win);
        w_gotoxy_puts(Chat_win, 10, message_num, username);
        wattron(Chat_win, COLOR_PAIR(3));
        w_gotoxy_puts(Chat_win, 10 + strlen(username), message_num, pack.msg);
        wattron(Chat_win, COLOR_PAIR(4));
        w_gotoxy_puts(Chat_win, 1, message_num, timestr);
        message_num++;
    }
    wrefresh(Chat_win);
}

void send_chat() {
    echo();
    nocbreak();
    bzero(&send_pack.msg, sizeof(send_pack.msg));
    bzero(&send_pack.to_name, sizeof(send_pack.to_name));
    pthread_mutex_lock(&draw_lock);
    wattron(Write_win, COLOR_PAIR(1));
    w_gotoxy_puts(Write_win, 1, 1, "输入消息 : ");
    char temp_msg[128] = {0};
    wattron(Write_win, COLOR_PAIR(3));
    mvwscanw(Write_win, 2, 1, "%[^\n]s", temp_msg);
    send_pack.type = PUBLIC_CHAT;
    int len = strlen(temp_msg);
    if (temp_msg[0] == '@') {
        char temp_name[20] = {0};
        int j = 0;
        for (int i = 1; i < len; i++) {
            if (temp_msg[i] != ':') {
                temp_name[j++] = temp_msg[i];
            } else {
                strcpy(send_pack.to_name, temp_name);
                for (int k = j + 2, l = 0; l < len; k++) {
                    if (k < len) send_pack.msg[l++] = temp_msg[k];
                    else send_pack.msg[l++] = '\0';
                }
                break;
            }
        }
        if (strlen(send_pack.msg)) {
            send_pack.type = PRIVATE_CHAT;
        }
    } else strcpy(send_pack.msg, temp_msg);
    wattron(Write_win, COLOR_PAIR(5));
    if (strlen(send_pack.msg)) {
        send(sockfd, &send_pack, sizeof(send_pack), 0);
    }
    wclear(Write_win);
    box(Write_win, 0, 0);
    wrefresh(Write_win);
    noecho();
    cbreak();
    pthread_mutex_unlock(&draw_lock);
    return ;
}

void *user_draw(void *argc) {
    int l = 1;
    pthread_mutex_lock(&draw_lock);
    wclear(Friend_win);
    pthread_mutex_lock(&lock);
    char online_num[1024] = {0};
    box(Friend_win, 0, 0);
    wattron(Friend_win, COLOR_PAIR(1));
    sprintf(online_num,"当前在线----%d人", (int)online_user.size());
    w_gotoxy_puts(Friend_win, 1, 1, online_num);
    wattron(Friend_win, COLOR_PAIR(4));

    for (auto i : online_user) {
        if (l > 22) break;
        w_gotoxy_puts(Friend_win, 1, ++l, i.first.data());
    }
    wattron(Friend_win, COLOR_PAIR(3));
    pthread_mutex_unlock(&lock);
    wrefresh(Friend_win);
    pthread_mutex_unlock(&draw_lock);
}
