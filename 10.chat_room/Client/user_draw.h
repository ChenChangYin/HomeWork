/*************************************************************************
	> File Name: user_draw.h
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月26日 星期四 17时49分00秒
 ************************************************************************/

#ifndef _USER_DRAW_H
#define _USER_DRAW_H
#include "../Common/head.h"
#include "./client_class.h"
void destroy_win(WINDOW *win);
void gotoxy(int x, int y);
void gotoxy_putc(int x, int y, char c);
void gotoxy_puts(int x, int y, const char *s);
void w_gotoxy_putc(WINDOW *win, int x, int y, int c);
void w_gotoxy_puts(WINDOW *win, int x, int y, const char *s);
void send_chat();
void show_Chat_win(Cli::Pack pack);
void *user_draw(void *argc);
void initfootball();
#endif
