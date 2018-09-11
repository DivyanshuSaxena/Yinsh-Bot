/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */

#include "Board.h"



double Board::getLinearMarkers() {
    double h;
    int player1 = 4;
    int player2 = 5;

    // Vertical Rows for both opponents
    int rowskone1 = 0;
    int rowsktwo1 = 0;
    int rowskone2 = 0;
    int rowsktwo2 = 0;
    for (int i = 0; i <= 2*n; i++) {
        int startj = i>n ? (i-n) : 0;
        int completej = i<n ? (n+i) : 2*n+1;
        int prevMarkerVert = config[i][startj];
        int prevMarkerHorz = config[startj][i];
        int countVert = 1, countHorz = 1;
        for (int j = startj+1; j <= completej; j++) {
            // Vertical Rows
            if (config[i][j] == prevMarkerVert) {
                if (!(prevMarkerVert == player1 || prevMarkerVert == player2) ) continue;
                countVert++;
                if (countVert == k-2) {
                    if (prevMarkerVert == player1) rowsktwo1++;
                    else rowsktwo2++;
                } else if (countVert == k-1) {
                    if (prevMarkerVert == player1) {
                        rowskone1++;
                        rowsktwo1--;
                    } else {
                        rowskone2++;
                        rowsktwo2--;
                    }
                }
            } else {
                if (config[i][j] == player1 || config[i][j] == player2) {
                    countVert = 1;
                } 
                prevMarkerVert = config[i][j];
            }
            
            // Horizontal Rows
            if (config[j][i] == prevMarkerHorz) {
                if (!(prevMarkerHorz == player1 || prevMarkerHorz == player2) ) continue;
                countHorz++;
                if (countHorz == k-2) {
                    if (prevMarkerHorz == player1) rowsktwo1++;
                    else rowsktwo2++;
                } else if (countHorz == k-1) {
                    if (prevMarkerHorz == player1) {
                        rowskone1++;
                        rowsktwo1--;
                    } else {
                        rowskone2++;
                        rowsktwo2--;
                    }
                }
            } else {
                if (config[j][i] == player1 || config[j][i] == player2) {
                    countHorz = 1;
                } 
                prevMarkerHorz = config[j][i];
            }
        }
    }

    return h;
}

double Board::getEvaluation() {
    double h;

    return h;
}