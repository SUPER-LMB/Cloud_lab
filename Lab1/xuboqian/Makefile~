CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku

sudoku:  ThreadPool.cpp  sudoku_dancing_links.cpp main1.cpp
	g++ -O2 -pthread -o  $@ $^
