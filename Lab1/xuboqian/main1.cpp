#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include "sudoku.h"
#include "ThreadPool.h"

using namespace std;
int board[1005][85];
int total_solved = 0;
int total = 0;
bool (*solve)(int) = solve_sudoku_dancing_links;
int leng=0;
pthread_mutex_t mutex;


int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

void *ans(void* i)
{
      /*pthread_mutex_lock(&mutex);    
	total_solved++;    
	printf("ok: %d\n", (int)i);
	pthread_mutex_unlock(&mutex);*/
      if (solve((int)i)) {
	pthread_mutex_lock(&mutex);        
	++total_solved;
	pthread_mutex_unlock(&mutex);	
        //if (!solved())
          //assert(0);
      }
      else {
        printf("No answer!");}
}


int main(int argc, char* argv[])
{
  int temp = sysconf(_SC_NPROCESSORS_CONF);
  pthread_mutex_init(&mutex,NULL);
  FILE* fp = fopen(argv[1], "r");
  char puzzle[128];
	
  pool_init(temp);
  int64_t start = now();
  while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
    if (strlen(puzzle) >= N) {
	pthread_mutex_lock(&mutex);
	for(int i=0;i<81;i++)
            board[total][i]=(puzzle[i]-'0');
	pthread_mutex_unlock(&mutex);
	pool_add_worker(ans, (void*)total);
	++total;
    }
  }
  int64_t end = now();
  double sec = (end-start)/1000000.0;
  sleep(1);
  
  pool_destroy();
  for(int i=0;i<total;i++)
  {
	cout<<"No "<<i+1<<":";
        for(int j=0;j<81;j++) cout<<board[i][j];
	cout<<endl;
  }
	printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
  return 0;
}

