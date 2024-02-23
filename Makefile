CXX = g++
CXXFLAGS=-std=c++11 -Wall -pedantic -g
LDFLAGS=-g -Wall -pedantic
LD = g++

all: sudoku

sudoku: main.o Sudoku.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f *.o

main.o: main.cpp Sudoku.h
Sudoku.o: Sudoku.cpp Sudoku.h