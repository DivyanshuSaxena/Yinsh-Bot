/* 
 * File:   Board.h
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
#ifndef Board_H
#define	Board_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class Board{
    public:
        int ** config;
        int n, m, k, l;  
        // n is hexagon size
        // m is the number of rings each player has
        // k is number of markers in line each player has to create to remove k markers
        // l is number of rings one player must remove before other player to win
        int ** getConfig();
        
        // Initialization
        Board();
        Board(int n, int m, int k, int l);

        // Accessor Methods
        pair <int, int> getCoordinates(int, int);
        vector<pair<int,int>> showpossiblemoves(int, int);
        vector<pair<int,int>> getFreePointsAdjacentToPoint(pair<int,int>, int);
        pair<int,int> getPairAfterMarkers(pair<int,int>, int);
        void printnormalconfig();
        
        // Operate on rings and markers on board
        bool addRing(int, int, int);
        bool setMarker(pair<int,int>, int);
        bool selectAndMoveRing(int, int, int, int);
        bool removeMarkers(int, int, int, int);
        bool removeRing(int, int);
        pair<int, int> getHexagonalCoordinate(int, int);
        void printhexagonalconfig();
};
#endif /* Board_H */