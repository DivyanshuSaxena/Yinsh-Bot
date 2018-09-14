/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */

#include "State.h"

State::State(Board* board) {
    // Duplicate the argument board
    stboard = new Board(n,m,k,l);
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<2*n+1;i++){
            stboard->config[i][j] = board->config[i][j];
        }
    }

    heuristic = -DBL_MAX;
    kInRow = false;
    resetFeatures();
}

void State::resetFeatures() {
    int rowsk1 = -1, rowsk2 = -1;
    int rowskone1 = -1, nonFlipRowskone1 = -1;
    int rowsktwo1 = -1, nonFlipRowsktwo1 = -1;
    int rowskone2 = -1, nonFlipRowskone2 = -1;
    int rowsktwo2 = -1, nonFlipRowsktwo2 = -1;
}

void State::setWeight(double weight, int feature) {
    if (feature < weights.size() && feature > 0) {
        weights.at(feature) = weight;
    }
}

void State::incrementkRows(int marker, bool flip, int endx, int endy) {
    if (marker == 4) {
        rowsk1++;
    } else {
        rowsk2++;
    }
    endkx = endx;
    endky = endy;
    kInRow = true;
}

void State::incrementRows(int count, int marker, bool flip) {
    // Increments all the in-a-row heuristic variables 
    // for the State object based on the count and the player marker
    if (count == k-2) {
        if (marker == 4) {
            rowsktwo1++;
            if (!flip) nonFlipRowsktwo1++;
        } else {
            rowsktwo2++;
            if (!flip) nonFlipRowsktwo2++;
        } 
    } else if (count == k-1) {
        if (marker == 4) {
            rowskone1++;
            if (!flip) {
                nonFlipRowskone1++;
            }
        } else {
            rowskone2++;
            if (!flip) {
                nonFlipRowskone2++;
            }
        }
    } else {
        // Presently, the code should never get into this -> 
        // k markers checked in getLinearMarkers
        cout << "Unreachable piece of code reached" << endl;
        incrementkRows(marker, flip, -1, -1);
    }
}

void State::getLinearMarkers() {
    int player1 = 4;
    int player2 = 5;

    // Vertical and Horizontal Rows for both opponents
    for (int i = 0; i <= 2*n; i++) {
        int startj = i>n ? (i-n) : 0;
        int completej = i<=n ? (n+i) : 2*n+1;
        int prevMarkerVert = stboard->config[i][startj];
        int prevMarkerHorz = stboard->config[startj][i];
        int countVert = 1, countHorz = 1;
        // Variables to check if the current streak is non-flippable or not
        bool flipVert = false, flipHorz = false; 
        int j = startj+1;
        for (j = startj+1; j < completej; j++) {
            // Vertical Rows
            if (stboard->config[i][j] == prevMarkerVert) {
                if (!(prevMarkerVert == player1 || prevMarkerVert == player2) ) goto horizontal;
                countVert++;
                cout << "Checking flipability" << endl;
                flipVert = flipVert || stboard->isFlippable(i, j);
            } else {
                if (stboard->config[i][j] == player1 || stboard->config[i][j] == player2) {
                    if (countVert >= k-2 && countVert <= k-1) 
                        incrementRows(countVert, prevMarkerVert, flipVert);
                    if (countVert >= k) incrementkRows(prevMarkerVert, flipVert, i, j);
                    countVert = 1;
                    startkx = i;
                    startky = j; // Set the start indices of streak start
                } 
                prevMarkerVert = stboard->config[i][j];
                flipVert = false;
            }
            cout << "Checked vertical for " << i << " " << j << endl; // Debug

            // Horizontal Rows
            horizontal:
            if (stboard->config[j][i] == prevMarkerHorz) {
                if (!(prevMarkerHorz == player1 || prevMarkerHorz == player2) ) continue;
                countHorz++;
                flipHorz = flipHorz || stboard->isFlippable(j, i);
            } else {
                if (stboard->config[j][i] == player1 || stboard->config[j][i] == player2) {
                    if (countHorz >= k-2 && countVert <= k-1) 
                        incrementRows(countHorz, prevMarkerHorz, flipHorz);
                    if (countHorz >= k) incrementkRows(prevMarkerHorz, flipHorz, j, i);
                    countHorz = 1;
                    startkx = j;
                    startky = i; // Set the start indices of streak start
                } 
                prevMarkerHorz = stboard->config[j][i];
                flipHorz = false;
            }
            cout << "Checked horizontal for " << j << " " << i << endl; // Debug
        }
        
        // Check for the last place
        if (countVert >= k-2 && countVert <= k-1) 
            incrementRows(countVert, prevMarkerVert, flipVert);
        if (countVert >= k) incrementkRows(prevMarkerVert, flipVert, i, j);

        if (countHorz >= k-2 && countVert <= k-1) 
            incrementRows(countHorz, prevMarkerHorz, flipHorz);
        if (countHorz >= k) incrementkRows(prevMarkerHorz, flipHorz, j, i);
    }

    // Slant Rows for both opponents
    bool flip = false;
    for (int diff = -n; diff <= n; diff++) {
        // Let the slant line be x-y = c, then diff is the iterator for c
        int startj = (diff<=0) ? 0 : diff;
        int endj = (diff<=0) ? 10 : (10-diff);
        int prevMarker = stboard->config[startj][startj-diff];
        int count = 0;
        int j = startj+1;
        for (j = startj+1; j <= endj; j++) {
            if (stboard->config[j][j-diff] == prevMarker) {
                if (prevMarker != player1 && prevMarker != player2) continue;
                count++;
                flip = flip || stboard->isFlippable(j, j-diff);
            } else {
                if (stboard->config[j][j-diff] == player1 || stboard->config[j][j-diff] == player2) {
                    if (count >= k-2 && count <= k-1) 
                        incrementRows(count, prevMarker, flip);
                    if (count > k) incrementkRows(prevMarker, flip, j, j-diff);
                    count = 1;
                    startkx = j;
                    startky = j-diff; // Set the start indices of streak start
                } 
                prevMarker = stboard->config[j][j-diff];
                flip = false;
            }
        }
        if (count >= k-2 && count <= k-1) 
            incrementRows(count, prevMarker, flip);
        if (count > k) incrementkRows(prevMarker, flip, j, j-diff);
    }
}

double State::weightedSum() {
    double h;
    // Rows of k-2 markers
    if (rowsktwo1 != -1) h += rowsktwo1 * weights.at(1);
    if (rowsktwo2 != -1) h += rowsktwo2 * weights.at(2);

    // Rows of k-2 non flippable markers
    if (nonFlipRowsktwo1 != -1) h += nonFlipRowsktwo1 * weights.at(3); 
    if (nonFlipRowsktwo2 != -1) h += nonFlipRowsktwo2 * weights.at(4); 
    
    // Rows of k-1 markers
    if (rowskone1 != -1) h += rowskone1 * weights.at(5);
    if (rowskone2 != -1) h += rowskone2 * weights.at(6);

    // Rows of k-1 non flippable markers
    if (nonFlipRowskone1 != -1) h += nonFlipRowskone1 * weights.at(7);
    if (nonFlipRowskone2 != -1) h += nonFlipRowskone2 * weights.at(8);

    // Rows of k markers
    if (rowsk1 != -1) h += rowsk1 * weights.at(9);
    if (rowsk2 != -1) h += rowsk2 * weights.at(10);
    return h;
}

bool State::isTerminalNode() {
    // Assumption -> It shall never happen that 
    // one player has no available moves while the other player has
    bool retVal = false;
    stboard->updateRingPositions();
    if (stboard->p1Rings.size() == m-l || stboard->p2Rings.size() == m-l) {
        retVal = true;
    } else {
        retVal = true;
        for (pair<int,int> ring : stboard->p1Rings) {
            vector<pair<int,int>> moves = stboard->showPossibleMoves(ring.first, ring.second);
            if (moves.size() != 0) {
                retVal = false;
                break;
            }
        }
    }
    return retVal;
}

double State::alphaBeta(int depth, int alpha, int beta){
    if(depth==0 || this->isTerminalNode()){
        return this->getEvaluation();
    }
    double tempscore = -DBL_MAX;
    vector<State *> successsors = this->getSuccessors();
    for(int i=0;i<successsors.size();i++){
        double value = -successsors[i]->alphaBeta(depth-1,-beta,-alpha);
        if(value>tempscore){
            tempscore=value;
        }
        if(tempscore>alpha){
            alpha=tempscore;
        }
        if(tempscore>=beta){
            break;
        }
    }
    return tempscore;
}

vector<State*> State::getSuccessors(){
    
}

/*
 * evaluate() evaluates the current state and checks ->
 * if there exist any row of k markers, in which case, it returns false
 * else returns true 
 */
bool State::evaluate() {
    // Assumption - At a time, only a single row of k markers can be present
    cout << "Starting Evaluation" << endl; // Debug
    this->getLinearMarkers();
    if (kInRow) {
        return false;
    }
    double h = weightedSum();
    cout << h << endl;
    heuristic = h;
    return true;
}

double State::getEvaluation() {
    return heuristic;
}
