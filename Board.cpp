/* 
 * File:   Board.cpp
 * Author: Akshat Khare
 * 
 */

//player 1 ring is 2 , player 2 ring is 3, player 1 marker is 4, player 2 marker is 5
//for now we are considering conventional boards

#include "Board.h"

int ** Board::getConfig(){
    return this->config;
}
Board::Board(){
    //for now we are considering conventional boards
    int ** tempconfig = new int*[11];
    for(int i=0;i<11;i++){
        tempconfig[i] = new int[11];
        for(int j=0;j<11;j++){
            tempconfig[i][j] = 0;
        }
    }
    tempconfig[5][5] = 1;
    for(int i=1;i<=5;i++){
        if(i!=5){
            tempconfig[5][5+i]=1;
            tempconfig[5+i][5+i]=1;
            tempconfig[5+i][5]=1;
            tempconfig[5][5-i]=1;
            tempconfig[5-i][5-i]=1;
            tempconfig[5-i][5]=1;
        }
        for(int j=0;j<i-1;j++){
            tempconfig[6+j][5+i]=1;
            tempconfig[4-j][5-i]=1;
            tempconfig[5+i][6+j]=1;
            tempconfig[5-i][4-j]=1;
            tempconfig[6-i+j][6+j]=1;
            tempconfig[4+i-j][4-j]=1;
        }
    }
    this->config = tempconfig;
}
void Board::printnormalconfig(){
    for(int j=10;j>=0;j--){
        for(int i=0;i<11;i++){
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
            return make_pair(5,5);
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
                return make_pair(5+rempos, 5+hexagon);
            case 1:
                return make_pair(5+hexagon, 5+hexagon-rempos);
            case 2:
                return make_pair(5+hexagon -rempos, 5-rempos);
            case 3:
                return make_pair(5-rempos, 5- hexagon);
            case 4:
                return make_pair(5-hexagon, 5-hexagon+rempos);
            case 5:
                return make_pair(5-hexagon+rempos,5+rempos);
            default:
                return make_pair(-1,-1);
        }
    }
}
vector<pair<int,int>> Board::showpossiblemoves(int hexagon, int position){
    return null;
}