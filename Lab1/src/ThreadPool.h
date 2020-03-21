#include <iostream>
#include <fstream>
#include <pthread.h>
#include <cstring>
#include <vector>
using namespace std;

typedef void (*Function)(int);

class ThreadPool{
    public:
        ThreadPool(int MaxNum,int total,Function task);
        ~ThreadPool();
        static void *running(void *);
        Function  func;
        int MaxThreadNum;
        int totalnum;
        pthread_mutex_t lock;
        int curpos;
};