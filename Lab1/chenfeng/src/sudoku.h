#ifndef SUDOKU_H
#define SUDOKU_H
#include <vector>
#include <pthread.h>
using namespace std;
// const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
// const int NUM = 9;

extern pthread_mutex_t vectorlock;
// extern int neighbors[N][NEIGHBOR];

extern int CurPos;//当前访问的board位置
extern vector<vector<int> > board;
// extern int spaces[N];
// extern int nspaces;
// extern int (*chess)[COL];

// void init_neighbors();
// void input(const char in[N]);
// void init_cache();

bool available(int guess, int cell);

// bool solve_sudoku_basic(int which_space);
// bool solve_sudoku_min_arity(int which_space);
// bool solve_sudoku_min_arity_cache(int which_space);
bool solve_sudoku_dancing_links(int);
//bool solved();
#endif
