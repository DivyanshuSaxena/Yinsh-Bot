/* 
 * File:   Board.h
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
#ifndef BOARD_H
#define	BOARD_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

extern int n, m, k, l;  
// n is hexagon size
// m is the number of rings each player has
// k is number of markers in line each player has to create to remove k markers
// l is number of rings one player must remove before other player to win

class Board{
    public:
        int ** config;
        vector<pair<int,int>> p1Rings;
        vector<pair<int,int>> p2Rings;
        int ** getConfig();
        
        // Initialization
        Board();
        Board(int, int, int, int);

        // Accessor Methods
        pair <int, int> getCoordinates(int, int);
        vector<pair<int,int>> showPossibleMoves(int, int);
        vector<pair<int,int>> getFreePointsAdjacentToPoint(pair<int,int>, int);
        pair<int,int> getPairAfterMarkers(pair<int,int>, int);
        void updateRingPositions();
        void printnormalconfig();
        
        // Operate on rings and markers on board
        bool addRing(int, int, int);
        pair<int,int> makeInitialMoves(int);
        pair<int,int> blockOpponentRings();

        bool setMarker(pair<int,int>, int);
        bool selectAndMoveRing(int, int, int, int);
        bool removeMarkers(int, int, int, int);
        bool removeRing(int, int);
        bool isFlippable(int, int);
        pair<int, int> getHexagonalCoordinate(int, int);
        void printhexagonalconfig();
};

extern Board* board;
extern int player_id;

#endif /* Board_H */