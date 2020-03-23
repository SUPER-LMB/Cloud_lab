#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#include "sudoku.h"
#include "multi_thread.h"

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
  init_neighbors();

  FILE* fp = fopen(argv[1], "r");
  char puzzle[128];
  bool (*solve)(char * , int) = solve_sudoku_basic;
  if (argv[2] != NULL)
    if (argv[2][0] == 'a')
      solve = solve_sudoku_min_arity;
    else if (argv[2][0] == 'c')
      solve = solve_sudoku_min_arity_cache;
    else if (argv[2][0] == 'd')
      solve = solve_sudoku_dancing_links;
  make_mission(fp, solve) ;
  
  int64_t start = now();

#if (!MULTI_THREAD && !MULTI_THREAD_2)
  for(int i = 0 ; i < total ; i++){
    args tmp_args ;
    tmp_args.solve_name = solve ;
    tmp_args.m = &mission_queue[i] ;
    tmp_args.which_space = 0 ;
    solve_sudoku((void*)&tmp_args) ;
  }
#endif

#if (MULTI_THREAD && !MULTI_THREAD_2)
  for(int i = 0 ; i < mission_queue.size() ; i++){
    // args tmp_args ;
    // tmp_args.solve_name = solve ;
    // tmp_args.m = &mission_queue[i] ;
    // tmp_args.which_space = 0 ;
    // args_queue.push_back(tmp_args) ;
    // int loc = args_queue.size() ;

    args_queue[i].solve_name = solve ;
    args_queue[i].m = &mission_queue[i] ;
    args_queue[i].which_space = 0 ;
    pthread_t tid ;
    pthread_create(&tid, NULL, solve_sudoku, (void*)&args_queue[i]) ;
    thread_queue.push_back(tid) ;
  }
  // sleep(3) ;
  for(int i = 0 ; i < thread_queue.size() ; i++){
    pthread_join(thread_queue[i], NULL) ;
  }
#endif

#if (!MULTI_THREAD && MULTI_THREAD_2)

  args tmp_arg ;
  tmp_arg.solve_name = solve ;
  for(int i = 0 ; i < SYS_THREAD_NUM ; i++){
    pthread_t tid ;
    printf("%d is creating", i+1) ;
    pthread_create(&tid, NULL, solve_thread, (void*)&tmp_arg) ;
    printf("%d created", i+1) ;
    thread_queue.push_back(tid) ;
  }
  for(int i = 0 ; i < SYS_THREAD_NUM ; i++){
    pthread_join(thread_queue[i], NULL) ;
  }
#endif

  int64_t end = now();
  double sec = (end-start)/1000000.0;
  printf("\n--------------------------------------------------------------------\n") ;
  for(int i = 0 ; i < mission_queue.size() ; i++){
    // if(mission_queue[i].sovle)
      printf("%s", mission_queue[i].puzzle) ;
    // else 
    //   printf("false %d\n", i+1) ;
  }
  printf("\n--------------------------------------------------------------------\n") ;
  printf("total %f sec, %f ms each sudoku problem, solved %d\n", sec, 1000*sec/total, total_solved);
  return 0;
}

