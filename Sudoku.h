#include <iostream>
#include <vector>
#include <map>


#ifndef SEMESTRAL_WORK_SUDOKU_H
#define SEMESTRAL_WORK_SUDOKU_H

#define MAX_ITERATIONS 10000
#define EULER 2.71828


typedef int m_Value;

using namespace std;

class Sudoku {
    int                             m_Size{};

public:

    int   getSize() const;

    Sudoku() = default;

    void resize(int size);

    void print();

    void simulatedAnnealing();

    vector<vector<m_Value>> generateInitialState();

    void hillClimbing();

    vector<vector<m_Value>> neighbour(vector<vector<m_Value>> &vec);

    int fitness(vector<vector<m_Value>> &vec);

    void steepestAscending();

    double probability(int fitness1, int fitness2, double max);

    double random_double();

    void initArray(vector<vector<m_Value>> &vec);

    vector<vector<m_Value>>            m_Array;

    void dropTemperature(int i, double &temp, double param, int &numOfIterations);

    int getNumberOfIterations();

    double initialTemperature(vector<vector<m_Value>> &vec, vector<vector<m_Value>> &nb);
};


#endif //SEMESTRAL_WORK_SUDOKU_H
