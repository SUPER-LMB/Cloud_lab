#include <stdio.h>

#ifndef SUDOKU_H
#define SUDOKU_H

#define DEBUG_RES false
#define DEBUG_MODE false
#define MULTI_THREAD false
#define MULTI_THREAD_2 true

const int SYS_THREAD_NUM = 4 ;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

extern int neighbors[N][NEIGHBOR];

void init_neighbors();
void init_cache();

bool available(int guess, int cell);

bool solve_sudoku_basic(char puzzle[N], int which_space);
bool solve_sudoku_min_arity(char puzzle[N], int which_space);
bool solve_sudoku_min_arity_cache(char puzzle[N], int which_space);
bool solve_sudoku_dancing_links(char puzzle[N], int unused);
bool solved(int board[N]);
#endif
