#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#include "sudoku.h"
#include "multi_thread.h"

#define OutputMode 1 //打印标志位

using namespace std ;

int SYS_THREAD_NUM = 6 ;

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
  init_neighbors();
  SYS_THREAD_NUM = sysconf(_SC_NPROCESSORS_CONF) ;
  //printf("%d\n",SYS_THREAD_NUM);
  // FILE* fp = fopen(argv[1], "r");
  char puzzle[128];
  bool (*solve)(char * , int) = solve_sudoku_dancing_links;
  if (argv[1] != NULL)
    if (argv[1][0] == 'a')
      solve = solve_sudoku_min_arity;
    else if (argv[1][0] == 'c')
      solve = solve_sudoku_min_arity_cache;
    else if (argv[1][0] == 'b')
      solve = solve_sudoku_basic;
  file file_tmp ;
  // if(argv[1]!=NULL){
  //   file_list.push_back(file_tmp) ;
  //   file_size++ ;
  //   strcpy(file_list[file_size-1].file_name, argv[1]) ;
  // }

  char file_name[128] ;
  while(1){
    //char base[128]="./TestData/";
    cin.getline(file_name, 128) ;
    if(strcmp(file_name, "") == 0){
      break ;
    }
    else {
      file_list.push_back(file_tmp) ;
      file_size++ ;
      //strcat(base,file_name);
      //strcpy(file_name,base);
      strcpy(file_list[file_size-1].file_name, file_name) ;
    }
  }
  FILE *fp ;
  
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
  pthread_t input_tid ;
  pthread_create(&input_tid, NULL, input_mission_file, NULL) ;
  while(1){
    pthread_mutex_lock(&file_input_m) ;
    if(file_now == file_size) {
      pthread_mutex_unlock(&file_input_m) ;
      break ;
    }
    else {
      fp = fopen(file_list[file_now].file_name, "r");
      pthread_mutex_unlock(&file_input_m) ;
      make_mission(fp, solve) ;
      fclose(fp) ;
      args tmp_arg ;
      tmp_arg.solve_name = solve ;


      for(int i = 0 ; i < SYS_THREAD_NUM ; i++){
        pthread_t tid ;
        pthread_create(&tid, NULL, solve_thread, (void*)&tmp_arg) ;
        thread_queue.push_back(tid) ;
      }


      for(int i = 0 ; i < SYS_THREAD_NUM ; i++){
        pthread_join(thread_queue[i], NULL) ;
      }


      if(OutputMode) output_mission() ;
      file_now++ ;
    }
  }

#endif

  int64_t end = now();
  double sec = (end-start)/1000000.0;

// //output code
//   for(int i = 0 ; i < mission_queue.size() ; i++){
//     // if(mission_queue[i].sovle)
//       printf("%s", mission_queue[i].puzzle) ;
//     // else 
//     //   printf("false %d\n", i+1) ;
//   }

  //printf("total %f sec, %f ms each sudoku problem, solved %d\n", sec, 1000*sec/total, total_solved);
  return 0;
}

