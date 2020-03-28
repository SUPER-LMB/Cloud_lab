#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//任务接口(节点)
/*struct Work {
    void* (*func) (void *arg);
    void *arg;
    Work *next;
};

//线程池中线程执行的回掉函数
void* thread_poll_func(void *arg);

//线程池
struct thread_poll{
    //任务队列
    Work *head;
    //线程指针
    pthread_t *pd;

    //条件变量和锁
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    //线程池是否被销毁
    bool shutdown;

    //线程池中线程的总量
    int pthread_num;
    //当前任务队列中任务数量
    int work_num;

    //初始化函数
    thread_poll(int pthread_num) {
        this -> pthread_num = pthread_num;
        this -> work_num = 0;
        this -> shutdown = 0;
        pthread_mutex_init(&this ->mutex, NULL);
        pthread_cond_init(&this -> cond, NULL);
        this -> head = NULL;
        pd = (pthread_t *)malloc(sizeof(pthread_t) * pthread_num);
        for (int i = 0; i < pthread_num; i++) {
            pthread_create(&this -> pd[i], NULL, thread_poll_func, this);
        }
    }

    //向线程池投递任务
    void add_work(Work *work);

    //销毁线程池
    int del();
};*/

void pool_init(int max_thread_num);
int pool_add_worker(void *(*process)(void *arg), void *arg);
int pool_destroy();

