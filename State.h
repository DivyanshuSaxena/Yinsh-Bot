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
#include "TimeHelper.h"
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
        int blockDoF1, blockDoF2;

        int startkx, startky;
        int endkx, endky;
        bool kInRow;
        bool rings1Blocked, rings2Blocked;
        double heuristic;

        void incrementkRows(int, bool, int, int);
        void incrementRows(int, int, bool);
        void checkCount(int, int, bool, int, int, int, int);

    public:
        State(Board* board, int player);
        // State(State* state);
        Board* stboard;
        int playerToMove;
        bool isSuccessorsUpdated;
        int bestMove; // Index of best successor in successors
        vector<string> moves;
        vector<State*> successors;
        void setWeight(double weight, int feature);

        // Accessor Methods
        bool isTerminalNode();
        vector<State*> getSuccessors(int currPlayer);
        pair<vector<State*>, vector<string>> getStatesForMoves(int currPlayer, string);
        vector<pair< pair<int,int>, pair<int,int>>> getPossibleMarkerRemovals();

        // Operations
        void resetFeatures();
        void duplicateFeatures(State* state);
        double alphaBeta(int depth, double alpha, double beta, int currPlayer, int evSign);
        double iterativeDeepening(int max_depth, int playerId);
        void makeMove(); 
        string parseMove(int, int, int, int, int);

        // Heuristics
        void getLinearMarkers();
        void getBlockedRings();
        double weightedSum();
        bool evaluate(); 
        double getEvaluation();
};
#endif /* State_H */