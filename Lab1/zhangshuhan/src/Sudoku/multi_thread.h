#include<pthread.h>
#include<vector>
#include <stdio.h>
#include <string.h>

#include "sudoku.h"
#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

extern int total_solved ;
extern int total ;
extern int next_mission ;

extern pthread_mutex_t total_m ;
extern pthread_mutex_t total_s_m ;

struct mission{
    bool sovle = false ;
    char puzzle[128] ;
    int flag ;
};

struct args{
    bool (*solve_name)(char* , int) ;
    mission *m ;
    int which_space ;
};

extern std::vector<mission> mission_queue ; //vector does not support multi threads
extern std::vector<pthread_t> thread_queue ;
extern std::vector<args> args_queue ;
// extern mission mission_queue[1005] ;
// extern args args_queue[1005] ;


void make_mission(FILE *fp, bool (*solve)(char * , int)) ;
void* solve_sudoku(void *arg) ;
void* solve_thread(void *arg) ;
#endif