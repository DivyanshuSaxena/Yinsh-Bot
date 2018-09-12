/* 
 * File:   State.h
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
#ifndef State_H
#define	State_H

#include <iostream>
#include <fstream>
#include <vector>
#include "Board.h"
using namespace std;

class State {
    private:
        int rowskone1, nonFlipRowskone1;
        int rowsktwo1, nonFlipRowsktwo1;
        int rowskone2, nonFlipRowskone2;
        int rowsktwo2, nonFlipRowsktwo2;

        void incrementRows(int, int, bool);
    public:
        // Heuristics
        double getLinearMarkers();
        double getEvaluation();
};
#endif /* State_H */