#include <iostream>
#include <fstream>
#include <pthread.h>
#include <cstring>
#include <vector>
using namespace std;

typedef void (*Function)(int);

class ThreadPool{
    public:
        ThreadPool(int MaxNum,string filename,Function task);
        ~ThreadPool();
        void CreateThread();
        void WaitThread();
        static void *running(void *);
        Function  func;
        ifstream instream;
        int MaxThreadNum;
        //int totalnum;
        pthread_mutex_t lock;
        //int curpos;
        int is_open;//记录文件是否成功打开
        
};