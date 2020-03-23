#include <assert.h>

#include <algorithm>

#include "sudoku.h"

struct sudoku_min_arity
{
  /* data */
  int board[N];
  int spaces[N];
  int nspaces;
  // int (*chess)[COL] = (int (*)[COL])board;


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

  bool available(int guess, int cell)
  {
    for (int i = 0; i < NEIGHBOR; ++i) {
      int neighbor = neighbors[cell][i];
      if (board[neighbor] == guess) {
        return false;
      }
    }
    return true;
  }

  int arity(int cell)
  {
    bool occupied[10] = {false};
    for (int i = 0; i < NEIGHBOR; ++i) {
      int neighbor = neighbors[cell][i];
      occupied[board[neighbor]] = true;
    }
    return std::count(occupied+1, occupied+10, false);
  }

  void find_min_arity(int space)
  {
    int cell = spaces[space];
    int min_space = space;
    int min_arity = arity(cell);

    for (int sp = space+1; sp < nspaces && min_arity > 1; ++sp) {
      int cur_arity = arity(spaces[sp]);
      if (cur_arity < min_arity) {
        min_arity = cur_arity;
        min_space = sp;
      }
    }

    if (space != min_space) {
      std::swap(spaces[min_space], spaces[space]);
    }
  }

  bool solve_sudoku_min_arity(int which_space)
  {
    if (which_space >= nspaces) {
      return true;
    }

    find_min_arity(which_space);
    int cell = spaces[which_space];

    for (int guess = 1; guess <= NUM; ++guess) {
      if (available(guess, cell)) {
        // hold
        assert(board[cell] == 0);
        board[cell] = guess;

        // try
        if (solve_sudoku_min_arity(which_space+1)) {
          return true;
        }

        // unhold
        assert(board[cell] == guess);
        board[cell] = 0;
      }
    }
    return false;
  }

};


bool solve_sudoku_min_arity(char puzzle[N], int which_space){
  sudoku_min_arity min ;
  min.input(puzzle) ;
  bool a = min.solve_sudoku_min_arity(which_space) ;
  if (!solved(min.board) && a == true)
    assert(0);
  if (a){
    min.get_result(puzzle) ;
#if DEBUG_RES
    printf("%s", puzzle) ;
#endif 
  }
  return a ;
}