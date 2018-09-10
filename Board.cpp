/* 
 * File:   Board.cpp
 * Author: Akshat Khare
 * 
 */

// player 1 ring is 2 , player 2 ring is 3, player 1 marker is 4, player 2 marker is 5

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
    // cout << "aloha" <<endl;
    auto freevecinfislope = this->getFreePointsAdjacentToPoint(thisringpair, 90);
    auto freevecminusinfislope = this->getFreePointsAdjacentToPoint(thisringpair, 270);
    auto freeveczeroslope = this->getFreePointsAdjacentToPoint(thisringpair, 0);
    auto freevecminuszeroslope = this->getFreePointsAdjacentToPoint(thisringpair, 180);
    auto freeveconeslope = this->getFreePointsAdjacentToPoint(thisringpair, 45);
    auto freevecminusoneslope = this->getFreePointsAdjacentToPoint(thisringpair, 225);
    myvec.insert(myvec.end(), freevecinfislope.begin(),freevecinfislope.end());
    myvec.insert(myvec.end(), freevecminusinfislope.begin(),freevecminusinfislope.end());
    myvec.insert(myvec.end(), freeveczeroslope.begin(),freeveczeroslope.end());
    myvec.insert(myvec.end(), freevecminuszeroslope.begin(),freevecminuszeroslope.end());
    myvec.insert(myvec.end(), freeveconeslope.begin(),freeveconeslope.end());
    myvec.insert(myvec.end(), freevecminusoneslope.begin(),freevecminusoneslope.end());

    auto freepointinfislope = this->getPairAfterMarkers(thisringpair, 90);
    auto freepointminusinfislope = this->getPairAfterMarkers(thisringpair, 270);
    auto freepointzeroslope = this->getPairAfterMarkers(thisringpair, 0);
    auto freepointminuszeroslope = this->getPairAfterMarkers(thisringpair, 180);
    auto freepointoneslope = this->getPairAfterMarkers(thisringpair, 45);
    auto freepointminusoneslope = this->getPairAfterMarkers(thisringpair, 225);
    if(freepointinfislope.first != -1 && freepointinfislope.second != -1){
        myvec.push_back(freepointinfislope);
    }
    if(freepointminusinfislope.first != -1 && freepointminusinfislope.second != -1){
        myvec.push_back(freepointminusinfislope);
    }
    if(freepointzeroslope.first != -1 && freepointzeroslope.second != -1){
        myvec.push_back(freepointzeroslope);
    }
    if(freepointminuszeroslope.first != -1 && freepointminuszeroslope.second != -1){
        myvec.push_back(freepointminuszeroslope);
    }
    if(freepointoneslope.first != -1 && freepointoneslope.second != -1){
        myvec.push_back(freepointoneslope);
    }
    if(freepointminusoneslope.first != -1 && freepointminusoneslope.second != -1){
        myvec.push_back(freepointminusoneslope);
    }
    return myvec;
}
vector<pair<int,int>> Board::getFreePointsAdjacentToPoint(pair<int,int> argpair, int slope){
    vector<pair<int,int>> myvec;
    int tempi = argpair.first;
    int tempj = argpair.second;
    if(slope==90){
        while(true && tempj<2*this->n){
            tempj++;
            // cout << "tempi is "<<tempi<< " tempj is "<< tempj<<endl;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                // cout<< "lets push " << myvec.size()<<endl;
                myvec.push_back(make_pair(tempi,tempj));
            }
        }
    }else if(slope==270){
        while(true && tempj>0){
            tempj--;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                myvec.push_back(make_pair(tempi,tempj));
            }
        }
    }else if(slope==0 ){
        while(true && tempi < 2 * this->n){
            tempi++;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                myvec.push_back(make_pair(tempi,tempj));
            }
        }
    }else if(slope == 180){
        while(true && tempi > 0){
            tempi--;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                myvec.push_back(make_pair(tempi,tempj));
            }
        }
    }else if(slope == 45){
        while(true && tempi < 2 * this->n && tempj < 2 * this->n){
            tempi++;
            tempj++;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                myvec.push_back(make_pair(tempi,tempj));
            }
        }
    }else if(slope = 225){
        while(true && tempi >0 && tempj >0){
            tempi--;
            tempj--;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                myvec.push_back(make_pair(tempi,tempj));
            }
        }
    }
    // cout << "finvec size is "<< myvec.size()<<endl;
    return myvec;
}
pair<int,int> Board::getPairAfterMarkers(pair<int,int> argpair, int slope){
    int tempi = argpair.first;
    int tempj = argpair.second;
    if(slope==90){
        if(this->config[tempi][tempj+1]>3){
            tempj++;
            while(true && tempj<this->n*2){
                tempj++;
                if(this->config[tempi][tempj]!=4 && this->config[tempi][tempj]!=5){
                    break;
                }
            }
            if(this->config[tempi][tempj]==1){
                return make_pair(tempi,tempj);
            }else{
                return make_pair(-1,-1);
            }
        }else{
            return make_pair(-1,-1);
        }
    }else if(slope==270){
        if(this->config[tempi][tempj-1]>3){
            tempj--;
            while(true && tempj>0){
                tempj--;
                if(this->config[tempi][tempj]!=4 && this->config[tempi][tempj]!=5){
                    break;
                }
            }
            if(this->config[tempi][tempj]==1){
                return make_pair(tempi,tempj);
            }else{
                return make_pair(-1,-1);
            }
        }else{
            return make_pair(-1,-1);
        }
    }else if(slope==0){
        if(this->config[tempi+1][tempj]>3){
            tempi++;
            while(true && tempi<this->n*2){
                tempi++;
                if(this->config[tempi][tempj]!=4 && this->config[tempi][tempj]!=5){
                    break;
                }
            }
            if(this->config[tempi][tempj]==1){
                return make_pair(tempi,tempj);
            }else{
                return make_pair(-1,-1);
            }
        }else{
            return make_pair(-1,-1);
        }
    }else if(slope==180){
        if(this->config[tempi-1][tempj]>3){
            tempi--;
            while(true && tempi>0){
                tempi--;
                if(this->config[tempi][tempj]!=4 && this->config[tempi][tempj]!=5){
                    break;
                }
            }
            if(this->config[tempi][tempj]==1){
                return make_pair(tempi,tempj);
            }else{
                return make_pair(-1,-1);
            }
        }else{
            return make_pair(-1,-1);
        }
    }else if(slope==45){
        if(this->config[tempi+1][tempj+1]>3){
            tempi++;
            tempj++;
            while(true && tempi<this->n*2 && tempj<this->n*2){
                tempi++;
                tempj++;
                if(this->config[tempi][tempj]!=4 && this->config[tempi][tempj]!=5){
                    break;
                }
            }
            if(this->config[tempi][tempj]==1){
                return make_pair(tempi,tempj);
            }else{
                return make_pair(-1,-1);
            }
        }else{
            return make_pair(-1,-1);
        }
    }else if(slope==225){
        if(this->config[tempi-1][tempj-1]>3){
            tempi--;
            tempj--;
            while(true && tempi>0 && tempj>0){
                tempi--;
                tempj--;
                if(this->config[tempi][tempj]!=4 && this->config[tempi][tempj]!=5){
                    break;
                }
            }
            if(this->config[tempi][tempj]==1){
                return make_pair(tempi,tempj);
            }else{
                return make_pair(-1,-1);
            }
        }else{
            return make_pair(-1,-1);
        }
    }else{
        return make_pair(-1,-1);
    }
    return make_pair(-1,-1);
}
bool Board::setMarker(pair<int,int> argpair, int playerid){
    //not complete function yet
    if(playerid==2){
        this->config[argpair.first][argpair.second] = 4;
    }else if(playerid==3){
        this->config[argpair.first][argpair.second] = 5;
    }
    return true;
}