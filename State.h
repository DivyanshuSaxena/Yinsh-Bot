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
        // Number of rows with k markers for players 1 and 2
        int rowsk1, rowsk2;

        /*
        * Naming Convention for in-a-row variables:
        * <rows or nonFlipRows> + k + <one or two> + <player_number>
        * <one or two> denotes (k-1) and (k-2) markers, respectively
        * 
        * Non-flip markers are the non-flippable in the move next to the current state
        */
        int rowskone1, nonFlipRowskone1;
        int rowsktwo1, nonFlipRowsktwo1;
        int rowskone2, nonFlipRowskone2;
        int rowsktwo2, nonFlipRowsktwo2;

        void incrementRows(int, int, bool);
    public:
        int * weights;

        // Heuristics
        void getLinearMarkers();
        double getEvaluation();
};
#endif /* State_H */