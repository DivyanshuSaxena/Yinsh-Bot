/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */

// player 1 ring is 2 , player 2 ring is 3, player 1 marker is 4, player 2 marker is 5

#include "Board.h"

pair<int, int> backB7(vector<pair<int,int>> myvec){
    if(myvec.size()!=0){
        return myvec.back();
    }else{
        return make_pair(-1,-1);
    }
}

pair<int,int> getDirectionVector(pair<int,int> inipoint, pair<int,int> finpoint){
    // IT IS FROM FIRST ARGUMENT TO SECOND 
    int xdiff = finpoint.first - inipoint.first;
    int ydiff = finpoint.second - inipoint.second;
    if(xdiff==0){
        if(ydiff>0){
            return make_pair(0,1);
        }else if(ydiff<0){
            return make_pair(0,-1);
        }
    }else if(ydiff==0){
        if(xdiff>0){
            return make_pair(1,0);
        }else if(xdiff<0){
            return make_pair(-1,0);
        }
    }else if(xdiff==ydiff){
        if(xdiff>0){
            return make_pair(1,1);
        }else if(xdiff<0){
            return make_pair(-1,-1);
        }
    }
    return make_pair(0,0);
}

int ** Board::getConfig(){
    return this->config;
}

Board::Board(){
    //for now we are considering conventional boards
    Board(5,5,5,3);
}

Board::Board(int n0, int m0, int k0, int l0){
    // General boards
    n = n0;
    m = m0;
    k = k0;
    l = l0;

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

pair<int,int> Board::makeInitialMoves(int movenum) {
    pair<int,int> retPair;
    if (movenum == 1 && config[n][n+1] == 1) {
        retPair = make_pair(n,n+1);
    } else if (movenum <= m/2) {
        pair<int,int> ring = player_id==1 ? p2Rings.back() : p1Rings.back();
        
        bool madeMove = false;
        if (ring.second >= n) {
            if (config[ring.first+1][ring.second-1] == 1) {
                retPair = make_pair(ring.first+1, ring.second-1);
                madeMove = true;
            } else if (config[ring.first-1][ring.second-2] == 1) {
                retPair = make_pair(ring.first-1, ring.second-2);
                madeMove = true;
            } else if (config[ring.first-2][ring.second-1] == 1) {
                retPair = make_pair(ring.first-2, ring.second-1);
                madeMove = true;
            }
        } 
        if (!madeMove) {
            if (config[ring.first-1][ring.second+1] == 1) {
                retPair = make_pair(ring.first-1, ring.second+1);
                madeMove = true;
            } else if (config[ring.first+1][ring.second+2] == 1) {
                retPair = make_pair(ring.first+1, ring.second+2);
                madeMove = true;
            } else if (config[ring.first+2][ring.second+1] == 1) {
                retPair = make_pair(ring.first+2, ring.second+1);
                madeMove = true;
            }
        }
    } else 
        retPair = blockOpponentRings();

    bool check = addRing(player_id, retPair.first, retPair.second);
    if (!check) cout << "Some error in adding ring" << endl; // Debug
    return retPair;
}

pair<int,int> Board::blockOpponentRings() {
    // Make move so as to block possible rows of opponent rings
}

void Board::updateRingPositions(){
    p1Rings.clear();
    p2Rings.clear();
    for (int i=0;i<2*n+1;i++){
        for (int j=0;j<2*n+1;j++){
            if (config[i][j] == 2){
                p1Rings.push_back(make_pair(i,j));
            } else if (config[i][j] == 3){
                p2Rings.push_back(make_pair(i,j));
            }
        }
    }
}

void Board::printnormalconfig(){
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<2*n+1;i++){
            cout<< this->config[i][j] << " ";
        }
        cout<< endl;
    }
}

bool Board::addRing(int player, int xarg, int yarg){
    if(xarg==-1 || yarg ==-1){
        return false;
    }else if(this->config[xarg][yarg]==1){
        // player 1 ring is 2 , player 2 ring is 3
        if(player==1){
            this->config[xarg][yarg] = 2;
            p1Rings.push_back(make_pair(xarg, yarg));
            return true;
        }else if(player==2){
            this->config[xarg][yarg] = 3;
            p2Rings.push_back(make_pair(xarg, yarg));
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

vector<pair<int,int>> Board::showPossibleMoves(int hexagon, int position){
    vector<pair<int,int>> myvec;
    auto thisringpair = this->getCoordinates(hexagon,position);
    int ringnum = this->config[thisringpair.first][thisringpair.second];
    auto freevecinfislope = this->getFreePointsAdjacentToPoint(thisringpair, 90);
    auto skipfreevecinfislope = this->getPairAfterMarkers(backB7(freevecinfislope), 90);
    auto freevecminusinfislope = this->getFreePointsAdjacentToPoint(thisringpair, 270);
    auto skipfreevecminusinfislope = this->getPairAfterMarkers(backB7(freevecminusinfislope), 270);
    auto freeveczeroslope = this->getFreePointsAdjacentToPoint(thisringpair, 0);
    auto skipfreeveczeroslope = this->getPairAfterMarkers(backB7(freeveczeroslope), 0);
    auto freevecminuszeroslope = this->getFreePointsAdjacentToPoint(thisringpair, 180);
    auto skipfreevecminuszeroslope = this->getPairAfterMarkers(backB7(freevecminuszeroslope), 180);
    auto freeveconeslope = this->getFreePointsAdjacentToPoint(thisringpair, 45);
    auto skipfreeveconeslope = this->getPairAfterMarkers(backB7(freeveconeslope), 45);
    auto freevecminusoneslope = this->getFreePointsAdjacentToPoint(thisringpair, 225);
    auto skipfreevecminusoneslope = this->getPairAfterMarkers(backB7(freevecminusoneslope), 225);
    myvec.insert(myvec.end(), freevecinfislope.begin(),freevecinfislope.end());
    myvec.insert(myvec.end(), freevecminusinfislope.begin(),freevecminusinfislope.end());
    myvec.insert(myvec.end(), freeveczeroslope.begin(),freeveczeroslope.end());
    myvec.insert(myvec.end(), freevecminuszeroslope.begin(),freevecminuszeroslope.end());
    myvec.insert(myvec.end(), freeveconeslope.begin(),freeveconeslope.end());
    myvec.insert(myvec.end(), freevecminusoneslope.begin(),freevecminusoneslope.end());

    // insert issue #1 case

    if(skipfreevecinfislope.first != -1 && skipfreevecinfislope.second != -1){
        myvec.push_back(skipfreevecinfislope);
    }
    if(skipfreevecminusinfislope.first != -1 && skipfreevecminusinfislope.second != -1){
        myvec.push_back(skipfreevecminusinfislope);
    }
    if(skipfreeveczeroslope.first != -1 && skipfreeveczeroslope.second != -1){
        myvec.push_back(skipfreeveczeroslope);
    }
    if(skipfreevecminuszeroslope.first != -1 && skipfreevecminuszeroslope.second != -1){
        myvec.push_back(skipfreevecminuszeroslope);
    }
    if(skipfreeveconeslope.first != -1 && skipfreeveconeslope.second != -1){
        myvec.push_back(skipfreeveconeslope);
    }
    if(skipfreevecminusoneslope.first != -1 && skipfreevecminusoneslope.second != -1){
        myvec.push_back(skipfreevecminusoneslope);
    }

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
        while(true && tempj<2*n){
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
        while(true && tempi < 2 * n){
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
        while(true && tempi < 2 * n && tempj < 2 * n){
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
    if(argpair.first == -1 || argpair.second==-1){
        return make_pair(-1,-1);
    }
    int tempi = argpair.first;
    int tempj = argpair.second;
    if(slope==90){
        // cout << "90" << endl;
        if(this->config[tempi][tempj+1]>3){
            tempj++;
            while(true && tempj<n*2){
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
        cout << "270" << endl;
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
            while(true && tempi<n*2){
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
            while(true && tempi<n*2 && tempj<n*2){
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

bool Board::selectAndMoveRing(int ringhexagon, int ringposition, int finringhexagon, int finringposition){
    //we are considering that the move is valid

    auto inipair = this->getCoordinates(ringhexagon, ringposition);
    auto destpair= this->getCoordinates(finringhexagon, finringposition);
    auto dirvec = getDirectionVector(inipair, destpair);
    // cout << "dirvec is "<< dirvec.first<< " "<<dirvec.second<<endl;
    int ringnum = this->config[inipair.first][inipair.second];
    int ringmarker = ringnum+2;
    // betweenMarkersThere is the contigous case
    bool betweenMarkersThere;
    if(this->config[inipair.first+dirvec.first][inipair.second+dirvec.second]>3){
        betweenMarkersThere = true;
    }else{
        betweenMarkersThere = false;
    }
    if(betweenMarkersThere){
        // cout<< "between markers true "<<endl;
        this->config[inipair.first][inipair.second] = ringmarker;
        int tempi = inipair.first+dirvec.first;
        int tempj = inipair.second+dirvec.second;
        while(this->config[tempi][tempj] == 4 || this->config[tempi][tempj]==5 ){
            this->config[tempi][tempj] = 9 - this->config[tempi][tempj];
            tempi = tempi + dirvec.first;
            tempj = tempj + dirvec.second;
        }
        // tempi,tempj = destpair at this point
        this->config[destpair.first][destpair.second] = ringnum;
        return true;
    }else{
        bool nonContinousMarkersThere;
        if(this->config[destpair.first-dirvec.first][destpair.second-dirvec.second]>3){
            nonContinousMarkersThere = true;
        }else{
            nonContinousMarkersThere = false;
        }
        if(nonContinousMarkersThere){
            // we will do end iteration here
            this->config[destpair.first][destpair.second]= ringnum;
            int tempi = destpair.first - dirvec.first;
            int tempj = destpair.second - dirvec.second;
            while(this->config[tempi][tempj] == 4 || this->config[tempi][tempj]==5 ){
                this->config[tempi][tempj] = 9 - this->config[tempi][tempj];
                tempi = tempi - dirvec.first;
                tempj = tempj - dirvec.second;
            }
            //tempi, tempj would not be equal to inipair
            this->config[inipair.first][inipair.second] = ringmarker;
            return true;
        }else{
            this->config[inipair.first][inipair.second] = ringmarker;
            this->config[destpair.first][destpair.second] = ringnum;
            return true;
        }
        
    }
}

bool Board::removeMarkers(int starthexagon, int starthexagonposition, int finhexagon, int finhexagonposition){
    auto inipair = this->getCoordinates(starthexagon, starthexagonposition);
    auto destpair= this->getCoordinates(finhexagon, finhexagonposition);
    auto dirvec = getDirectionVector(inipair, destpair);
    for(int i=0;i< k;i++){
        this->config[inipair.first + i*dirvec.first][inipair.second + i*dirvec.second] = 1;
    }
    return true;
}

bool Board::removeRing(int hexagon, int position){
    auto thispair = this->getCoordinates(hexagon, position);
    this->config[thispair.first][thispair.second] = 1;
}

bool Board::isFlippable(int row, int col){
    if (config[row][col] !=4 && config[row][col] != 5)
        return false;

    updateRingPositions();
    bool retVal = false;
    int flipMarker = this->config[row][col]==4 ? 3 : 2;
    for (int i = 0; i < m; i++) {
        if ((flipMarker==2 && i==board->p1Rings.size()) || (flipMarker==3 && i==board->p2Rings.size())) break;
        
        // Ring pair available at index i
        pair<int,int> ring = flipMarker==2 ? p1Rings.at(i) : p2Rings.at(i);
        int ringx = ring.first, ringy = ring.second;
        vector<pair<int,int>> possibleMoves = board->showPossibleMoves(ring.first, ring.second);
        for (int k = 0; k < possibleMoves.size(); k++) {
            pair<int,int> move = possibleMoves.at(k);
            int movex = move.first, movey = move.second;
            if ((movey-ringy)*(movex-row) == (movey-col)*(movex-ringx)) {
                retVal = true;
                break;
            }
        }
    }
    return retVal;
}

pair<int, int> Board::getHexagonalCoordinate(int xarg, int yarg){
    // xarg
    // give xarg and yarg in 0,0 coordinates
    if(xarg==0 && yarg==0){
        return make_pair(0,0);
    }else if(xarg>=0 && yarg>0){
        if(yarg>xarg){
            return make_pair(yarg, xarg);
        }else{
            return make_pair(xarg, xarg-yarg+1*xarg);
        }
    }else if(xarg>0 &&yarg<=0){
        return make_pair(xarg-yarg, -yarg+2*(xarg-yarg));
    }else if(xarg<=0 && yarg<0){
        if(xarg>yarg){
            return make_pair(-yarg, -xarg+3*(-yarg));
        }else{
            return make_pair(-xarg, yarg-xarg+4*(-xarg));
        }
    }else if(xarg<0 && yarg>=0){
        return make_pair(yarg-xarg,yarg+5*(yarg-xarg));
    }else{
        return make_pair(-1, -1);
    }
}

void Board::printhexagonalconfig(){
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<2*n+1;i++){
            // cout<< this->config[i][j] << " ";
            pair<int,int> temphex;
            if(this->config[i][j]==0){
                // temphex = make_pair(-1,-1);
                cout << "x" << " , "<< "x" << " | ";
            }else{
                temphex = this->getHexagonalCoordinate(i-n,j-n);
                cout << temphex.first << " , "<< temphex.second << " | ";
            }
            // cout << temphex.first << " , "<< temphex.second << " | ";
        }
        cout<< endl;
    }
}
