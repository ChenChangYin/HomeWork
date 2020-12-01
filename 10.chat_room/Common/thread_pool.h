/*************************************************************************
	> File Name: thread_pool.h
	> Author: 
	> Mail: 
	> Created Time: 2020年08月10日 星期一 10时25分55秒
 ************************************************************************/


#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include "./head.h"

struct Task {
    void *(*run)(void *arg);
    void *arg;
};

struct Thread_pool {
    pthread_cond_t have_task;
    pthread_cond_t over;
    pthread_mutex_t mutex;
    queue<Task> tasks;
    pthread_t *threads;
    int cnt, wait_cnt;
    int max_thread_cnt;
    bool exit_flag;
};

//处理任务
void *thread_run(void *arg);
//判断线程存活
bool thread_is_alive(pthread_t thread);
//加入任务
int add_task(Thread_pool *pool, void *(*run)(void *args), void *arg);
//清空
void thread_pool_clear(Thread_pool *pool);
//初始化线程池
void thread_pool_init(Thread_pool *pool);

#endif
