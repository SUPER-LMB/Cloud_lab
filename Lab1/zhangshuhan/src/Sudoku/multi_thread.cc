#include<pthread.h>
#include<vector>
#include <stdio.h>
#include <string.h>



#include "sudoku.h"
#include "multi_thread.h"

int total_solved = 0;
int total = 0;
int next_mission = 0;

pthread_mutex_t total_m ;
pthread_mutex_t total_s_m ;
pthread_mutex_t next_mission_m ;

std::vector<mission> mission_queue ; //vector does not support multi threads
std::vector<pthread_t> thread_queue ;
std::vector<args> args_queue ;
// mission mission_queue[1005] ;
// args args_queue[1005] ;

// vector version to make missions
void make_mission(FILE *fp, bool (*solve)(char * , int)){
    char puzzle[128] ;
    while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
        total++ ;
        if (strlen(puzzle) >= N) {
            mission tmp ;
            tmp.sovle = false ;
            tmp.flag = total ;
            strcpy(tmp.puzzle, puzzle) ;
            mission_queue.push_back(tmp) ;

            args tmp_args ;
            args_queue.push_back(tmp_args) ;

            // int tmp = mission_queue.size()-1 ;
            // args_queue[tmp].solve_name = solve ;
            // args_queue[tmp].m = &mission_queue[tmp] ;
            // args_queue[tmp].which_space = 0 ;
        }
    }
}

#if (!MULTI_THREAD && !MULTI_THREAD_2)
void* solve_sudoku(void *arg){
    args *tmp_args = (args*)arg ;
    bool (*solve)(char * , int) = tmp_args->solve_name ;
    if (strlen((tmp_args->m)->puzzle) >= N) {
        // ++total;
        if (solve((tmp_args->m)->puzzle, tmp_args->which_space)) {
            (tmp_args->m)->sovle = true ;
            ++total_solved;
        }
        else {
            printf("No: %s", (tmp_args->m)->puzzle);
        }
    }
}
#endif

#if (MULTI_THREAD && !MULTI_THREAD_2)
void* solve_sudoku(void *arg){
    args* tmp_args = (args*)arg ;
    bool (*solve)(char * , int) = tmp_args->solve_name ;
    if (strlen((tmp_args->m)->puzzle) >= N) {
        if (solve((tmp_args->m)->puzzle, tmp_args->which_space)) {
            pthread_mutex_lock(&total_s_m) ;
            (tmp_args->m)->sovle = true ;
            ++total_solved;
            pthread_mutex_unlock(&total_s_m) ;
        }
        else {
            printf("No: %s", (tmp_args->m)->puzzle);
        }
    }
}
#endif

#if (!MULTI_THREAD && MULTI_THREAD_2)
void* solve_sudoku(void* arg){
    args* tmp_args = (args*)arg ;
    bool (*solve)(char * , int) = tmp_args->solve_name ;
    if (strlen((tmp_args->m)->puzzle) >= N) {
        if (solve((tmp_args->m)->puzzle, tmp_args->which_space)) {
            pthread_mutex_lock(&total_s_m) ;
            (tmp_args->m)->sovle = true ;
            ++total_solved;
            pthread_mutex_unlock(&total_s_m) ;
        }
        else {
            printf("No: %s", (tmp_args->m)->puzzle);
        }
    }
}
void* solve_thread(void* arg){
    args *tmp_arg = (args *)arg ;
    bool (*solve)(char * , int) = tmp_arg->solve_name ;
    while (next_mission < total)
    {
        pthread_mutex_lock(&next_mission_m) ;
        int i = next_mission ;
        next_mission++ ;
        pthread_mutex_unlock(&next_mission_m) ;
        args_queue[i].solve_name = solve ;
        args_queue[i].m = &mission_queue[i] ;
        args_queue[i].which_space = 0 ;
        solve_sudoku(&args_queue[i]) ;
    }
}
#endif