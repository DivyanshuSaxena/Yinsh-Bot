/* 
 * File:   Board.h
 * Author: Akshat Khare
 * 
 */
#ifndef Board_H
#define	Board_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class Board{
    private:
        int ** config;
    public:
        int n, m, k, l;  
        // n is hexagon size
        // m is the number of rings each player has
        // k is number of markers in line each player has to create to remove k markers
        // l is number of rings one player must remove before other player to win
        int ** getConfig();
        Board();
        Board(int n, int m, int k, int l);
        void printnormalconfig();
        bool addRing(int player, int hexagon, int position);
        pair <int, int> getCoordinates(int, int);
        vector<pair<int,int>> showpossiblemoves(int, int);
        vector<pair<int,int>> getFreePointsAdjacentToPoint(pair<int,int> thispair, int slope);
        pair<int,int> getPairAfterMarkers(pair<int,int> thispair, int slope);
        bool setMarker(pair<int,int> thispair, int playermarker);
};
#endif /* Board_H */