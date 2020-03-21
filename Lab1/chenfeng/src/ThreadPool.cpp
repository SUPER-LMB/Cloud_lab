#include "ThreadPool.h"
#include "sudoku.h"
#include <pthread.h>
#include <fstream>
ThreadPool::ThreadPool(int MaxNum,string filename,Function task){
    this->MaxThreadNum=MaxNum;
    this->func=task;
    // this->curpos=0;
    // this->totalnum=total;
    this->is_open=1;
    this->instream.open("./datafiles/"+filename);
    if(!this->instream){
        cout<<"failed to open file "<<filename<<endl;
        this->is_open=0;
    }
    pthread_mutex_init(&lock,NULL);
    //this->thread=new pthread_t(MaxNum);
}

void ThreadPool::CreateThread(){
    //线程
    pthread_t thread[this->MaxThreadNum];
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
    for(int i=0;i<MaxThreadNum;i++){
        void *m;
        pthread_join(thread[i],(void**)&m);
    }
}

void ThreadPool::WaitThread(){
    // for(int i=0;i<MaxThreadNum;i++){
    //     void *m;
    //     pthread_join(this->thread[i],(void**)&m);
    // }
}

ThreadPool::~ThreadPool(){
    instream.close();
    pthread_mutex_destroy(&lock);
}

void* ThreadPool::running(void *arg){
    ThreadPool* Args = (ThreadPool*)arg;
    //cout<<"tid : "<<pthread_self()<<endl;
    while(1){
        int pos=0;
        string buf;
        pthread_mutex_lock(&Args->lock);
        if(getline(Args->instream,buf)){
            vector<int> tmp;
            for(int i=0;i<81;i++){
                tmp.push_back(buf[i]-'0');
            }
            board.push_back(tmp);
            pos=CurPos++;
        }
        else {
            pthread_mutex_unlock(&Args->lock);
            break;
        }
        pthread_mutex_unlock(&Args->lock);
        
        Args->func(pos);
    }
    return NULL;
}
