/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */

#include "State.h"

void State::incrementRows(int count, int marker, bool flip) {
    // Increments all the in-a-row heuristic variables for the State object based on the count and the player marker
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
            rowsktwo1--;
            if (!flip) {
                nonFlipRowskone1++;
                nonFlipRowsktwo1--;
            }
        } else {
            rowskone2++;
            rowsktwo2--;
            if (!flip) {
                nonFlipRowskone2++;
                nonFlipRowsktwo2--;
            }
        }
    } else {
        if (marker == 4) {
            rowsk1++;
            rowskone1--;
            rowsktwo1--;
        } else {
            rowsk2++;
            rowskone2--;
            rowsktwo2--;
        }
    }
}

void State::getLinearMarkers() {
    int player1 = 4;
    int player2 = 5;

    // Vertical and Horizontal Rows for both opponents
    for (int i = 0; i <= 2*n; i++) {
        int startj = i>n ? (i-n) : 0;
        int completej = i<n ? (n+i) : 2*n+1;
        int prevMarkerVert = board->config[i][startj];
        int prevMarkerHorz = board->config[startj][i];
        int countVert = 1, countHorz = 1;
        bool flipVert = false, flipHorz = false; // Variables to check if the current streak is non-flippable or not
        for (int j = startj+1; j <= completej; j++) {
            // Vertical Rows
            if (board->config[i][j] == prevMarkerVert) {
                if (!(prevMarkerVert == player1 || prevMarkerVert == player2) ) continue;
                countVert++;
                flipVert = flipVert || board->isFlippable(i, j);
                if (countVert >= k-2) incrementRows(countVert, prevMarkerVert, flipVert);
            } else {
                if (board->config[i][j] == player1 || board->config[i][j] == player2) {
                    countVert = 1;
                } 
                prevMarkerVert = board->config[i][j];
                flipVert = false;
            }

            // Horizontal Rows
            if (board->config[j][i] == prevMarkerHorz) {
                if (!(prevMarkerHorz == player1 || prevMarkerHorz == player2) ) continue;
                countHorz++;
                flipHorz = flipHorz || board->isFlippable(j, i);
                if (countHorz >= k-2) incrementRows(countHorz, prevMarkerHorz, flipHorz);
            } else {
                if (board->config[j][i] == player1 || board->config[j][i] == player2) {
                    countHorz = 1;
                } 
                prevMarkerHorz = board->config[j][i];
                flipHorz = false;
            }
        }
    }

    // Slant Rows for both opponents
    bool flip;
    for (int diff = -n; diff <= n; diff++) {
        // Let the slant line be x-y = c, then diff is the iterator for c
        int startj = (diff<=0) ? 0 : diff;
        int endj = (diff<=0) ? 10 : (10-diff);
        int prevMarker = board->config[startj][startj-diff];
        int count = 0;
        for (int j = startj+1; j <= endj; j++) {
            if (board->config[j][j-diff] == prevMarker) {
                if (prevMarker != player1 && prevMarker != player2) continue;
                count++;
                flip = flip || board->isFlippable(j, j-diff);
                if (count >= k-2) incrementRows(count, prevMarker, flip);
            } else {
                if (board->config[j][j-diff] == player1 || board->config[j][j-diff] == player2) {
                    count = 1;
                } 
                prevMarker = board->config[j][j-diff];
            }
        }
    }
}

double State::getEvaluation() {
    double h;
    
    return h;
}