#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <assert.h>
#include<unistd.h>
#include "sudoku.h"
#include "ThreadPool.h"

using namespace std;

int total_solved = 0;
int total = 0;
vector<vector<int> > board;

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

bool (*solve)(int) =  solve_sudoku_dancing_links;

void workfunc(int i){
    if (solve(i)) {
        ++total_solved;
    } else {
         cout << "no answer"<<endl;
    }
}

int main(int argc, char* argv[])
{
    //检测cpu核数
  int cpu_num;
  cpu_num = sysconf(_SC_NPROCESSORS_CONF);
  //cout<<cpu_num<<endl;
  vector<string>  filenames;
  string tmp;
  int64_t start = now();
  while(getline(cin,tmp)&&tmp!="\0"){
    
    filenames.push_back(tmp);
  }
  int64_t end = now();
  for(int i=0;i<(int)filenames.size();i++){
    ifstream in;
    in.open(filenames[i]);
    if(!in){
      cout<<"failed to open file "<<filenames[i]<<endl;
      exit(1);
    }
    string buf;
    while(getline(in,buf)){
      vector<int> tmp;
      for(int i=0;i<81;i++){
        tmp.push_back((buf[i]-'0'));
      }
      board.push_back(tmp);
      ++total;
    }
    in.close();
  }
  ThreadPool *t=new ThreadPool(cpu_num,board.size(),workfunc);
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
  return 0;
}
