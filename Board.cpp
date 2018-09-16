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

pair<int,int> Board::getDirectionVector(pair<int,int> inipoint, pair<int,int> finpoint){
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
    if (movenum == 1 && config[n][n] == 1) {
        outfile << "INITIAL MOVE: centre" << endl;
        retPair = make_pair(n,n);
    } else if (movenum <= (m+1)/2 || movenum <= (m-6)) {
        outfile << "INITIAL MOVE: rhombus" << endl;
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
    } else {
        outfile << "INITIAL MOVE: block" << endl;
        retPair = blockOpponentRings();
    }

    if (retPair.first < 0 || retPair.second < 0) {
        outfile << "Could not find a valid move for next ring, occupy corners" << endl; // Debug
        retPair = occupyCorners();
    }

    bool check = addRing(player_id, retPair.first, retPair.second);
    if (!check) outfile << "Some error in adding ring" << endl; // Debug
    outfile << "Init move: " << retPair.first << " " << retPair.second << endl; 
    pair<int,int> hexRetPair = board->getHexagonalCoordinate(retPair.first, retPair.second);
    return hexRetPair;
}

pair<int,int> Board::blockOpponentRings() {
    // Make move so as to block possible rows of opponent rings
    // Refine this function
    pair<int,int> ringi = player_id==1 ? p2Rings.back() : p1Rings.back();
    pair<int,int> retPair = make_pair(-1,-1);
    for (int j = 0; j < m; j++) {
        if ((player_id==1 && j==p2Rings.size()) || (player_id==2 && j==p1Rings.size())) break;

        pair<int,int> ringj = player_id==1 ? p2Rings.at(j) : p1Rings.at(j);
        // Check if rings in line - Block the most recently added one
        int xdiff = ringj.first - ringi.first;
        int ydiff = ringj.second - ringi.second;
        if (xdiff == 0) {
            int y = ydiff > 0 ? ringi.second+1 : ringi.second-1;
            if (config[ringi.first][y] != 1) continue;
            retPair = make_pair(ringi.first, y);
        } else if (ydiff == 0) {
            int x = xdiff > 0 ? ringi.first+1 : ringi.first-1;
            if (config[x][ringi.second] != 1) continue;
            retPair = make_pair(x, ringi.second);
        } else if (xdiff == ydiff) {
            int x = xdiff > 0 ? ringi.first+1 : ringi.first-1;
            int y = ydiff > 0 ? ringi.second+1 : ringi.second-1;
            if (config[x][y] != 1) continue;
            retPair = make_pair(x, y);
        }
    }
    return retPair;
}

pair<int,int> Board::occupyCorners() {
    int x = 0, y = 0;
    for (x = 1; x <= 9; x+=4) {
        bool found = false;
        for (y = 1; y <= 9; y+=4) {
            if (abs(x-y) > 5 || (x==5 && y==5)) continue;
            if (config[x][y] != 1) continue; 
            found=true;
            break;
        }
        if(found) break;
    }
    return make_pair(x,y);
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
            outfile<< this->config[i][j] << " ";
        }
        outfile<< endl;
    }
}
void Board::printBeautifiedconfig(){
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<10-j;i++){
            outfile << " ";
        }
        for(int i=0;i<2*n+1;i++){
            if(this->config[i][j]==0){
                outfile<< "  ";
            }else if(this->config[i][j]==1){
                outfile << "_ ";
            }else{
                outfile<< this->config[i][j] << " ";
            }
            
        }
        outfile<< endl;
    }
}
void Board::printMoreBeautifiedconfig(){
    // to write if needed
    int counter=0;
    while(counter<=20){
        if(counter<=5){

        }else if(counter<=10){

        }else if(counter<=15){

        }else{

        }
        counter++;
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

vector<pair<int,int>> Board::showPossibleMoves(int x, int y){
    vector<pair<int,int>> myvec;
    auto thisringpair = make_pair(x,y);
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
    // outfile << "Pair 1" << endl; // Debug

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
    // outfile << "Pair 2" << endl; // Debug

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
    // outfile << "Returning from showPossibleMoves" << endl; // Debug
    return myvec;
}

vector<pair<int,int>> Board::getFreePointsAdjacentToPoint(pair<int,int> argpair, int slope){
    vector<pair<int,int>> myvec;
    int tempi = argpair.first;
    int tempj = argpair.second;
    // outfile << "Finding adjacent points to " << tempi << " " << tempj << endl; // Debug
    if(slope==90){
        while(true && tempj<2*n){
            tempj++;
            // outfile << "tempi is "<<tempi<< " tempj is "<< tempj<<endl;
            if(this->config[tempi][tempj]!=1){
                break;
            }else{
                // outfile<< "lets push " << myvec.size()<<endl;
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
    // outfile << "finvec size is "<< myvec.size()<<endl;
    return myvec;
}

pair<int,int> Board::getPairAfterMarkers(pair<int,int> argpair, int slope){
    if(argpair.first == -1 || argpair.second==-1){
        return make_pair(-1,-1);
    }
    int tempi = argpair.first;
    int tempj = argpair.second;
    if(slope==90){
        if(tempj<2*n && this->config[tempi][tempj+1]>3){
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
        if(tempj>0 && this->config[tempi][tempj-1]>3){
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
        if(tempi<2*n && this->config[tempi+1][tempj]>3){
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
        if(tempi>0 && this->config[tempi-1][tempj]>3){
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
        if(tempi<2*n && tempj<2*n && config[tempi+1][tempj+1]>3){
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
        if(tempi>0 && tempj>0 && config[tempi-1][tempj-1]>3){
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

bool Board::selectAndMoveRing(int ringx, int ringy, int finringx, int finringy){
    // Assumption -> We are considering that the move is valid
    auto inipair = make_pair(ringx, ringy);
    auto destpair= make_pair(finringx, finringy);
    auto dirvec = getDirectionVector(inipair, destpair);
    // outfile << "dirvec is "<< dirvec.first<< " "<<dirvec.second<<endl;
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
        // outfile<< "between markers true "<<endl;
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

bool Board::removeMarkers(int startx, int starty, int finx, int finy){
    auto inipair = make_pair(startx, starty);
    auto destpair= make_pair(finx, finy);
    outfile << "inipair is "<<inipair.first << " "<<inipair.second<<endl;
    outfile << "destpair is "<<destpair.first << " "<<destpair.second<<endl;
    auto dirvec = getDirectionVector(inipair, destpair);
    outfile << "dirvec is "<<dirvec.first << " "<<dirvec.second<<endl;
    for(int i=0;i< k;i++){
        this->config[inipair.first + i*dirvec.first][inipair.second + i*dirvec.second] = 1;
    }
    return true;
}

bool Board::removeRing(int xarg, int yarg){
    if (config[xarg][yarg] == 2 || config[xarg][yarg] == 3) {
        this->config[xarg][yarg] = 1;
        return true;
    }
    return false;
}

bool Board::isFlippable(int row, int col){
    if (config[row][col] !=4 && config[row][col] != 5)
        return false;

    // updateRingPositions(); ALready called at start of evaluation
    bool retVal = false;
    int len = p1Rings.size() + p2Rings.size();
    for (int i = 0; i < len; i++) {
        // Ring pair available at index i
        pair<int,int> ring = i>=p1Rings.size() ? p2Rings.at(i-p1Rings.size()) : p1Rings.at(i);
        int ringx = ring.first, ringy = ring.second;
        vector<pair<int,int>> possibleMoves = board->showPossibleMoves(ring.first, ring.second);
        // outfile << "Check flip with ring: " << ringx << " " << ringy << " "; // Debug
        // outfile << "moves: " << possibleMoves.size() << endl;                // Debug
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

pair<int, int> Board::getHexagonalCoordinate(int x, int y){
    int xarg = x-5, yarg = y-5;
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
            // outfile<< this->config[i][j] << " ";
            pair<int,int> temphex;
            if(this->config[i][j]==0){
                // temphex = make_pair(-1,-1);
                outfile << "x" << " , "<< "x" << " | ";
            }else{
                temphex = this->getHexagonalCoordinate(i-n,j-n);
                outfile << temphex.first << " , "<< temphex.second << " | ";
            }
            // outfile << temphex.first << " , "<< temphex.second << " | ";
        }
        outfile<< endl;
    }
}
