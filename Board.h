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
        int ** getConfig();
        Board();
        void printnormalconfig();
        bool addRing(int , int , int);
        pair <int, int> getCoordinates(int, int);
        vector<pair<int,int>> showpossiblemoves(int, int);
};
#endif /* Board_H */