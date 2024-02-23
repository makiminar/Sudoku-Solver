#include <iostream>
#include <fstream>
#include "Sudoku.h"

int readInput(char *file, Sudoku & sudoku);


int main(int argc, char * argv[]) {

    if(argc != 2) {
        cerr << "2 arguments needed, provided " << argc << ".\n..." << endl << "Correct input: ./sudoku FILENAME"<< endl;
        return 1;
    }

    Sudoku sudoku = Sudoku();
    int x;

    if( ( x =  readInput(argv[1], sudoku)) < 0)
        return x;

    sudoku.steepestAscending();
    //sudoku.hillClimbing();
    //sudoku.simulatedAnnealing();

    sudoku.print();

    return 0;
}


// function for loading and parsing the input file
int readInput(char *file, Sudoku& sudoku) {

    ifstream input;
    input.open(file);

    if(!input) {
        cerr << "Error while attempting to read from file: " << file << endl;
        return -1;
    }

    char symbol;
    int num;

    input >> num;
    if(num < 9) {
        cout << "Sudoku dimension >= 9" << endl;
        return -2;
    }

    sudoku.resize(num);

    input >> symbol;
    if(symbol != ',') {
        cout << "Syntax error: first parameters are [number], [number]" << endl;
        return -3;
    }

    input >> num;
    if(num != sudoku.getSize()) {
        cout << "Logic error" << endl;
        return -4;
    }

    // load sudoku matrix

    int row = 0, col = 0;

    input >> symbol;

    while( !input.eof() ) {

        if(symbol >= '0' && symbol <= '9' ) {
            sudoku.m_Array[row][col] = (int)symbol - '0';
        } else {
            sudoku.m_Array[row][col] = (int)symbol - 'A' + 10;
        }


        if(col == sudoku.getSize() - 1) {
            col = 0;
            row++;
        } else {
            col++;
        }

        input >> symbol;
    }

    return 0;
}













