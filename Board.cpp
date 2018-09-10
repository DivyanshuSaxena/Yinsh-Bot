/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */

// player 1 ring is 2 , player 2 ring is 3, player 1 marker is 4, player 2 marker is 5
// for now we are considering conventional boards

#include "Board.h"

int ** Board::getConfig(){
    return this->config;
}
Board::Board(){
    //for now we are considering conventional boards
    Board(5,5,5,3);
}

Board::Board(int n0, int m0, int k0, int l0){
    // General boards
    this->n = n0;
    this->m = m0;
    this->k = k0;
    this->l = l0;

    int ** tempconfig = new int*[2*n+1];
    for(int i=0; i<2*n+1; i++){
        tempconfig[i] = new int[2*n+1];
        for(int j=0; j<2*n+1; j++){
            tempconfig[i][j] = 0;
        }
    }
    tempconfig[n][n] = 1;
    for(int i=1;i<=n;i++){
        if(i!=n){
            tempconfig[n][n+i]=1;
            tempconfig[n+i][n+i]=1;
            tempconfig[n+i][n]=1;
            tempconfig[n][n-i]=1;
            tempconfig[n-i][n-i]=1;
            tempconfig[n-i][n]=1;
        }
        for(int j=0;j<i-1;j++){
            tempconfig[n+1+j][n+i]=1;
            tempconfig[n-1-j][n-i]=1;
            tempconfig[n+i][n+1+j]=1;
            tempconfig[n-i][n-1-j]=1;
            tempconfig[n+1-i+j][n+1+j]=1;
            tempconfig[n-1+i-j][n-1-j]=1;
        }
    }
    this->config = tempconfig;
}

void Board::printnormalconfig(){
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<2*n+1;i++){
            cout<< this->config[i][j] << " ";
        }
        cout<< endl;
    }
}
bool Board::addRing(int player, int hexagon, int position){
    pair <int, int> coor = this->getCoordinates(hexagon, position);
    if(coor.first==-1 || coor.second ==-1){
        return false;
    }else if(this->config[coor.first][coor.second]==1){
        //player 1 ring is 2 , player 2 ring is 3
        
        if(player==1){
            this->config[coor.first][coor.second] = 2;
            return true;
        }else if(player==2){
            this->config[coor.first][coor.second] = 3;
            return true;
        }else{
            return false;
        }

    }else{
        return false;
    }
}
pair <int,int> Board::getCoordinates(int hexagon, int position){
    if(hexagon==0){
        if(position==0){
            return make_pair(n,n);
        }else{
            return make_pair(-1,-1);
        }
    }else{
        int domainpos = position/hexagon;
        int rempos = position%hexagon;
        switch(domainpos)
        {
            case 0:
                // if()
                return make_pair(n+rempos, n+hexagon);
            case 1:
                return make_pair(n+hexagon, n+hexagon-rempos);
            case 2:
                return make_pair(n+hexagon-rempos, n-rempos);
            case 3:
                return make_pair(n-rempos, n-hexagon);
            case 4:
                return make_pair(n-hexagon, n-hexagon+rempos);
            case 5:
                return make_pair(n-hexagon+rempos,n+rempos);
            default:
                return make_pair(-1,-1);
        }
    }
}
vector<pair<int,int>> Board::showpossiblemoves(int hexagon, int position){
    vector<pair<int,int>> myvec;
    auto thisringpair = this->getCoordinates(hexagon,position);
    int ringnum = this->config[thisringpair.first][thisringpair.second];
    auto freevecinfislope = this->getFreePointsAdjacentToPoint(thisringpair, 2);
    return myvec;
}
vector<pair<int,int>> Board::getFreePointsAdjacentToPoint(pair<int,int> argpair, int slope){
    vector<pair<int,int>> myvec;
    return myvec;
}