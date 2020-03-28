#ifndef SUDOKU_H
#define SUDOKU_H
#include <vector>

using namespace std;

const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

extern int board[1005][85];
extern int nspaces;
extern int (*chess)[COL];

void init_neighbors();
void input(const char in[N]);
void init_cache();

bool available(int guess, int cell);

bool solve_sudoku_basic(int which_space);
bool solve_sudoku_min_arity(int which_space);
bool solve_sudoku_min_arity_cache(int which_space);
bool solve_sudoku_dancing_links(int unused);
bool solved();
#endif
