/*************************************************************************
	> File Name: head.h
	> Author: ShiSan
	> Mail: 1303501514qq.com
	> Created Time: 2020年11月24日 星期二 10时41分27秒
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <curses.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <locale.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <wait.h>
#include <signal.h>
#include <math.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/file.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <errno.h>
#include <string>
#include <queue>
#include <map>

using namespace std;
#define MAX_MSG 1024

#define PUBLIC_CHAT 0 //公聊
#define PRIVATE_CHAT 1 //私聊
#define QUITE 2 //关闭
#define WALL 3 //广播
#define ONLINE 4 //上线通知
#define OFLINE 5 //下线通知

#endif
