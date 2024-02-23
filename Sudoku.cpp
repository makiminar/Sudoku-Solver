#include <random>
#include <unistd.h>
#include <cmath>
#include <set>
#include <algorithm>
#include "Sudoku.h"

int Sudoku::getSize() const {
    return m_Size;
}

void Sudoku::resize(int size) {
    m_Size = size;

    m_Array.resize(m_Size);
    for (int i = 0; i < m_Size; i++) {
        m_Array[i].resize(m_Size);
    }
}

void Sudoku::print() {

    int k = 1;
    cout << " -----------------------" << endl;
    for(int i = 0; i < m_Size; i++) {
        for(int j = 0; j < m_Size; j++) {

            if(j % (int)sqrt(m_Size) == 0)
                cout << "| ";
            if(m_Array[i][j] >= 0 && m_Array[i][j] <= 9) {
                cout << (char)(m_Array[i][j] + '0') << " ";
            } else {
                cout << (char)(m_Array[i][j] + 'A' - 10) << " ";
            }
        }
        cout << "|" <<endl;

        if(i + 1 == (int)sqrt(m_Size)*k) {
            cout << " -----------------------" << endl;
            k++;
        }
    }

}

void Sudoku::simulatedAnnealing() {

    srand( getpid());

    vector<vector<m_Value>> vec = generateInitialState();

    vector<vector<m_Value>> nb;

    int fitness1, fitness2, itCount = getNumberOfIterations(), iterations = 0; //, restart = 0, stuck = 0;
    double decreaseParam = 0.99, TMax, TMin = 0.0001; //, tmp;

    if((fitness1 = fitness(vec)) == 0) {
        m_Array = vec;
        return;
    }

    TMax = initialTemperature(vec, nb);
    //tmp = TMax;


    while(TMax >= TMin) {

        iterations++;
        nb = neighbour(vec);

        if(fitness1  == 0) {
            break;
        }

        if((fitness2 = fitness(nb)) < fitness1) {
            vec = nb;
            fitness1 = fitness2;
        } else {
            if(probability(fitness1, fitness2, TMax) >= random_double()) {
                vec = nb;
                fitness1 = fitness2;
            }
        }

        cout << "Current fitness: " << fitness1 << ", neighbour fitness: " << fitness2 << endl;

        dropTemperature(iterations, TMax, decreaseParam, itCount);

        // restarts
       /* if(fitness1 != 0 && TMax < TMin && restart < 10) {
            TMax = tmp;
            iterations = 0;
            restart++;
        }*/
    }

    m_Array = vec;
}

vector<vector<m_Value>> Sudoku::generateInitialState() {

    set<int> options;

    for (int i = 1; i <= m_Size; ++i) {
        options.insert(i);
    }

    int value;
    int location;
    vector<vector<m_Value>> init;

    initArray(init);

    int p = sqrt(m_Size), pos_i = 0, pos_j = 0, square = 0, param = sqrt(m_Size);

    while(pos_i != m_Size && pos_j != m_Size) {

        for(int i = pos_i; i < param; i++) {

            for (int j = pos_j; j < p; j++) {

                // erase fixed values from options
                if ((value = m_Array[i][j]) != 0) {
                    options.erase(value);
                    init[i][j] = value;
                }
            }
        }

        // fill subsquare with unique values
        for(int k = pos_i; k < param; k++) {

            for (int l = pos_j; l < p; l++) {

                if(m_Array[k][l] == 0) {
                    location = random() % options.size();
                    auto it(options.begin());
                    advance(it, location);
                    init[k][l] =  *it ;
                    options.erase(it);
                }
            }
        }

        // update params
        for (int m = 1; m <= m_Size; m++) {
            options.insert(m);
        }

        square++;

        if(square == m_Size) {
            break;
        }

        if((square % (int)sqrt(m_Size)) == 0 ) {
            pos_i += sqrt(m_Size);
            pos_j = 0;
            p = sqrt(m_Size);
            param += sqrt(m_Size);
        } else {
            p += sqrt(m_Size);
            pos_j += sqrt(m_Size);
        }

    }


    return init;
}


// classic Hill Climbing
void Sudoku::hillClimbing() {

    srand( getpid());

    vector<vector<m_Value>> vec = generateInitialState();
    vector<vector<m_Value>> nb(vec);

    int i = 0;
    int fit1, fit2;

    while( (fit1 = fitness(vec))  != 0 && i < MAX_ITERATIONS) {


        nb = neighbour(vec);
        fit2 = fitness(nb);

        cout << "Current fitness: " << fit1 << ", neighbour fitness: " << fit2 << endl;

        if( fit2 < fit1) {
            vec = nb;
        }


        i++;
    }

    if(fit1 == 0) {
        cout << "solution" << endl;
        m_Array = vec;
        return;
    }

    cout << "No solution" << endl << endl;

}


// randomly chooses quadrant and swaps 2 squares in that quadrant (subsquare)
vector<vector<m_Value>>  Sudoku::neighbour(vector<vector<m_Value>> & vec) {

    random_device randomDevice;


    m_Value temp;
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, quadrant;

    uniform_int_distribution<int> intDistribution1(0, (int)sqrt(m_Size) - 1);
    uniform_int_distribution<int> intDistribution2(0, m_Size - 1);
    // pick random subsquare
    quadrant = intDistribution2(randomDevice);

    x1 = intDistribution1(randomDevice);
    if(quadrant > 0) {
        x1 += (quadrant / (int)sqrt(m_Size))*(int)sqrt(m_Size);
    }
    y1 = intDistribution1(randomDevice);
    if(quadrant >= (int)sqrt(m_Size)) {
        y1 += (quadrant % (int)sqrt(m_Size))*(int)sqrt(m_Size);

    }

    while(m_Array[x1][y1] != 0) {
        x1 = intDistribution1(randomDevice) ;
        if(quadrant > 0) {
            x1 += (quadrant / (int)sqrt(m_Size))*(int)sqrt(m_Size);
        }
        y1 = intDistribution1(randomDevice);
        if(quadrant >= (int)sqrt(m_Size)) {
            y1 += (quadrant % (int)sqrt(m_Size))*(int)sqrt(m_Size);

        }
    }


    x2 = intDistribution1(randomDevice) ;
    if(quadrant > 0) {
        x2 += (quadrant / (int)sqrt(m_Size))*(int)sqrt(m_Size);
    }
    y2 = intDistribution1(randomDevice);
    if(quadrant >= (int)sqrt(m_Size)) {
        y2 += (quadrant % (int)sqrt(m_Size))*(int)sqrt(m_Size);

    }

    while(m_Array[x2][y2] != 0 && x2 != x1 && y2 !=y1) {
        x2 = intDistribution1(randomDevice) ;
        if(quadrant > 0) {
            x2 += (quadrant / (int)sqrt(m_Size))*(int)sqrt(m_Size);
        }
        y2 =intDistribution1(randomDevice);
        if(quadrant >= (int)sqrt(m_Size)) {
            y2 += (quadrant % (int)sqrt(m_Size))*(int)sqrt(m_Size);

        }
    }


    // swap
    vector<vector<m_Value>> neighbour = vec;
    temp = neighbour[x2][y2];
    neighbour[x2][y2] = neighbour[x1][y1];
    neighbour[x1][y1] = temp;

    // check if the fitness is better, if not return recursively function

    return neighbour;
}


// calculates number of collision/violations (duplicates) in rows and columns
int Sudoku::fitness(vector<vector<m_Value>> &vec) {

    int fitness = 0;
    m_Value val1, val2;

    for(int i = 0; i < m_Size; i++) {

        for(int j = 0; j < m_Size; j++) {

            val1 = vec[i][j];
            val2 = vec[j][i];

            for (int l = j + 1; l < m_Size; ++l) {

                if(vec[i][l] == val1)
                    fitness++;

                if(vec[l][i] == val2)
                    fitness++;
            }
        }
    }

    return fitness;
}

void Sudoku::steepestAscending() {

    srand( getpid());

    vector<vector<m_Value>> vec = generateInitialState();
    vector<vector<m_Value>> nb, neighbourCandidate;

    int i = 0;
    int fit1, fit2;

    while( (fit1 = fitness(vec)) != 0 && i < MAX_ITERATIONS) {

        cout << "Current fitness: " << fit1 ;

        nb = neighbour(vec);
        for (int j = 0; j < 10; ++j) {

            neighbourCandidate = neighbour(nb);

            if( fitness(neighbourCandidate) < fitness(nb))
                nb = neighbourCandidate;

        }


        if((fit2 = fitness(nb)) < fitness(vec))
            vec = nb;

        cout << ", best neighbour: " << fit2 << endl;


        i++;
    }

    if(fit1 == 0) {
        m_Array = vec;
    }
}


// calculates probability, which will be used to determine whether to accept worse solution or not
double Sudoku::probability(int fitness1, int fitness2, double max) {

    return pow(EULER,  -(fitness2 - fitness1) / max);

}


// returns random number in range <0,1>
double Sudoku::random_double() {
    random_device randomDevice;

    uniform_real_distribution<> dis(0.0, 1.0);

    return dis(randomDevice);
}


// initializes array
void Sudoku::initArray(vector<vector<m_Value>> &vec) {

    vec.resize(m_Size);
    for (int i = 0; i < m_Size; ++i) {
        vec[i].resize(m_Size);
    }
}


// function to drop the temperature after some specific number of iterations
void Sudoku::dropTemperature(int i, double &temp, double param, int &numOfIterations) {
    if(i % numOfIterations == 0) {
        temp *= param;
    }
    i = 0;
}


// calculates iterations for each sudoku - number of free squares
int Sudoku::getNumberOfIterations() {
    int it = 0;

    for (int i = 0; i < m_Size; ++i) {
        for (int j = 0; j < m_Size; ++j) {
            if(m_Array[i][j] == 0)
                it++;
        }
    }

    return it;
}


// calculates initial temperature
double Sudoku::initialTemperature(vector<vector<m_Value>> &vec, vector<vector<m_Value>> &nb) {

    vector<int> costs;

    // randomly generate neighbours and calculate their cost function
    for (int i = 0; i < 10; ++i) {
        nb = neighbour(vec);
        costs.push_back(fitness(nb));
    }

    // calculate standard deviation

    double sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i = 0; i < 10; ++i) {
        sum += costs[i];
    }

    mean = sum/10;

    for(i = 0; i < 10; ++i)
        standardDeviation += pow(costs[i] - mean, 2);

    return sqrt(standardDeviation / 10);
}
