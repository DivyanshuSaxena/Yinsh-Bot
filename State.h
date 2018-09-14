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
#include <cfloat>
#include <limits>
#include "Board.h"
using namespace std;

extern vector<double> weights;

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

        int startkx, startky;
        int endkx, endky;
        bool kInRow;
        double heuristic;

        void incrementkRows(int, bool, int, int);
        void incrementRows(int, int, bool);

    public:
        State(Board* board);
        Board* stboard;
        void setWeight(double weight, int feature);

        // Accessor Methods
        bool isTerminalNode();

        // Operations
        void resetFeatures();

        // Heuristics
        void getLinearMarkers();
        double weightedSum();
        bool evaluate(); 
        double getEvaluation();
        double alphaBeta(int depth, int alpha, int beta);
        vector<State*> getSuccessors();
};
#endif /* State_H */