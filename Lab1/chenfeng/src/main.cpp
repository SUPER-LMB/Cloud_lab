#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <assert.h>
#include<unistd.h>
#include<pthread.h>
#include "sudoku.h"
#include "ThreadPool.h"
#define answer 1
using namespace std;
pthread_mutex_t plock;
int total_solved = 0;
vector<vector<int> > board;
int CurPos=0;

pthread_mutex_t vectorlock;//避免vector改变数据和添加数据同时发生

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

bool (*solve)(int) =  solve_sudoku_dancing_links;

void workfunc(int i){
    if (solve(i)) {
        pthread_mutex_lock(&plock);
        ++total_solved;
        //cout<<total_solved<<endl;
        pthread_mutex_unlock(&plock);
    } else {
         cout << "no answer"<<endl;
    }
}

void printAns(){
  ofstream out;
  out.open("./datafiles/multhreadAns.txt");
  for(int i=0;i<CurPos;i++){
    for(int j=0;j<81;j++)out<<board[i][j];
    out<<endl;
  }
}

int main(int argc, char* argv[])
{
  pthread_mutex_init(&vectorlock,NULL);
    //检测cpu核数
  int cpu_num;
  cpu_num = sysconf(_SC_NPROCESSORS_CONF);
  pthread_mutex_init(&plock,NULL);
  //cout<<cpu_num<<endl;
  vector<string>  filenames;
  string tmp;

  //连续回车两次结束输入
  while(getline(cin,tmp)&&tmp!="\0"){
    
    filenames.push_back(tmp);
  }

  int64_t start = now();
  for(int i=0;i<(int)filenames.size();i++){
    // ifstream in;
    // in.open(filenames[i]);
    // if(!in){
    //   cout<<"failed to open file "<<filenames[i]<<endl;
    //   exit(1);
    // }
    // string buf;
    // while(getline(in,buf)){
    //   vector<int> tmp;
    //   for(int i=0;i<81;i++){
    //     tmp.push_back((buf[i]-'0'));
    //   }
    //   board.push_back(tmp);
    //   ++total;
    // }
    // in.close();
    ThreadPool *t=new ThreadPool(cpu_num,filenames[i],workfunc);
    if(t->is_open){
      t->CreateThread();
      //t->WaitThread();
    }
    //delete []t;
  }
  int64_t end = now();
  //ThreadPool *t=new ThreadPool(cpu_num,board.size(),workfunc);
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each %d\n", sec, 1000*sec/CurPos, total_solved);
  if(answer) printAns();
  pthread_mutex_destroy(&plock);
  pthread_mutex_destroy(&vectorlock);
  return 0;
}
