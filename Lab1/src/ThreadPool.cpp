#include "ThreadPool.h"
#include <pthread.h>
ThreadPool::ThreadPool(int MaxNum,int total,Function task){
    this->MaxThreadNum=MaxNum;
    this->func=task;
    this->curpos=0;
    this->totalnum=total;
    pthread_mutex_init(&lock,NULL);
    
    //线程
    pthread_t thread[MaxThreadNum];

    //创建线程
    pthread_mutex_lock(&lock);
    for(int i=0;i<MaxThreadNum;i++){
        int ret=pthread_create(&thread[i],NULL,running,this);
        while (ret != 0) {
            cout << "create pthread error! " << ret << "\n";
            ret = pthread_create(thread+ i, NULL,running, this);
        }
    }
    pthread_mutex_unlock(&lock);
    for(int i=0;i<MaxNum;i++){
        void *m;
        pthread_join(thread[i],(void**)&m);
    }
}

ThreadPool::~ThreadPool(){
    pthread_mutex_destroy(&lock);
}

void* ThreadPool::running(void *arg){
    ThreadPool* Args = (ThreadPool*)arg;
    //cout<<"tid : "<<pthread_self()<<endl;
    while(1){
        int pos=0;
        pthread_mutex_lock(&Args->lock);
        if(Args->curpos==Args->totalnum){
            pthread_mutex_unlock(&Args->lock);
            break;
        }
        else{
            pos=Args->curpos++;
        }
        pthread_mutex_unlock(&Args->lock);
        
        Args->func(pos);
    }
    return NULL;
}
