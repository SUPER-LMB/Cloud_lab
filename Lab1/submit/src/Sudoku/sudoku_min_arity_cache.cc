#include <assert.h>
#include <strings.h>

#include <algorithm>

#include "sudoku.h"

struct sudoku_min_arity_cache
{
  /* data */
  int board[N];
  int spaces[N];
  int nspaces;
  // int (*chess)[COL] = (int (*)[COL])board;

  bool occupied[N][NUM+1];
  int arity[N];

  void find_spaces()
  {
    nspaces = 0;
    for (int cell = 0; cell < N; ++cell) {
      if (board[cell] == 0)
        spaces[nspaces++] = cell;
    }
  }

  void input(const char in[N])
  {
    for (int cell = 0; cell < N; ++cell) {
      board[cell] = in[cell] - '0';
      assert(0 <= board[cell] && board[cell] <= NUM);
    }
    find_spaces();
  }

  void get_result(char * puzzle)
  {
    for (int cell = 0; cell < N; ++cell) {
      puzzle[cell] = board[cell] + '0';
      assert('0' <= puzzle[cell] && puzzle[cell] <= '9');
    }
  }

  // int arity(int cell)
  // {
  //   bool occupied[10] = {false};
  //   for (int i = 0; i < NEIGHBOR; ++i) {
  //     int neighbor = neighbors[cell][i];
  //     occupied[board[neighbor]] = true;
  //   }
  //   return std::count(occupied+1, occupied+10, false);
  // }
  void find_min_arity(int space)
  {
    int cell = spaces[space];
    int min_space = space;
    int min_arity = arity[cell];

    for (int sp = space+1; sp < nspaces && min_arity > 1; ++sp) {
      int cur_arity = arity[spaces[sp]];
      if (cur_arity < min_arity) {
        min_arity = cur_arity;
        min_space = sp;
      }
    }

    if (space != min_space) {
      std::swap(spaces[min_space], spaces[space]);
    }
  }

  void init_cache()
  {
    bzero(occupied, sizeof(occupied));
    std::fill(arity, arity + N, NUM);
    for (int cell = 0; cell < N; ++cell) {
      occupied[cell][0] = true;
      int val = board[cell];
      if (val > 0) {
        occupied[cell][val] = true;
        for (int n = 0; n < NEIGHBOR; ++n) {
          int neighbor = neighbors[cell][n];
          if (!occupied[neighbor][val]) {
            occupied[neighbor][val] = true;
            --arity[neighbor];
          }
        }
      }
    }
  }

  bool solve_sudoku_min_arity_cache(int which_space)
  {
    if (which_space >= nspaces) {
      return true;
    }

    find_min_arity(which_space);
    int cell = spaces[which_space];

    for (int guess = 1; guess <= NUM; ++guess) {
      if (!occupied[cell][guess]) {
        // hold
        assert(board[cell] == 0);
        board[cell] = guess;
        occupied[cell][guess] = true;

        // remember changes
        int modified[NEIGHBOR];
        int nmodified = 0;
        for (int n = 0; n < NEIGHBOR; ++n) {
          int neighbor = neighbors[cell][n];
          if (!occupied[neighbor][guess]) {
            occupied[neighbor][guess] = true;
            --arity[neighbor];
            modified[nmodified++] = neighbor;
          }
        }

        // try
        if (solve_sudoku_min_arity_cache(which_space+1)) {
          return true;
        }

        // unhold
        occupied[cell][guess] = false;
        assert(board[cell] == guess);
        board[cell] = 0;

        // undo changes
        for (int i = 0; i < nmodified; ++i) {
          int neighbor = modified[i];
          assert(occupied[neighbor][guess]);
          occupied[neighbor][guess] = false;
          ++arity[neighbor];
        }
      }
    }
    return false;
  }
};

bool solve_sudoku_min_arity_cache(char puzzle[N], int which_space){
  sudoku_min_arity_cache min_cache ;
  min_cache.input(puzzle) ;
  min_cache.init_cache();
  bool a = min_cache.solve_sudoku_min_arity_cache(which_space) ;
  if (!solved(min_cache.board) && a == true)
    assert(0);
  if (a){
    min_cache.get_result(puzzle) ;
#if DEBUG_RES
    printf("%s", puzzle) ;
#endif 
  }
  return a ;
}


