/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */

#include "State.h"

vector<pair<int,int>> copyVectorOfPairs(vector<pair<int,int>> thisvec){
    vector<pair<int,int>> tempring;
    for(int i=0;i<thisvec.size();i++){
        tempring.push_back(make_pair(thisvec[i].first, thisvec[i].second));
    }
    return tempring;
}

string State::parseMove(int movetype, int x1, int y1, int x2, int y2) {
    /*
     * Legend : movetype =>
     * 1 -> Move ring
     * 2 -> Remove row
     * 3 -> Remove ring
     */
    string str;
    pair<int,int> start = stboard->getHexagonalCoordinate(x1, y1);
    pair<int,int> end = stboard->getHexagonalCoordinate(x2, y2);
    string startx = to_string(start.first), starty = to_string(start.second);
    string endx = to_string(end.first), endy = to_string(end.second);
    if (movetype == 1) {
        str = "S " + startx + " " + starty + " M " + endx + " " + endy;
    } else if (movetype == 2) {
        str = "RS " + startx + " " + starty + " RE " + endx + " " + endy;
    } else {
        str = "X " + startx + " " + starty;
    }
    return str;
}

State::State(Board* board) {
    // Duplicate the argument board
    stboard = new Board(n,m,k,l);
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<2*n+1;i++){
            stboard->config[i][j] = board->config[i][j];
        }
    }
    // stboard->p1Rings = copyVectorOfPairs(board->p1Rings);
    // stboard->p2Rings = copyVectorOfPairs(board->p2Rings);
    stboard->updateRingPositions();
    heuristic = -DBL_MAX;
    kInRow = false;

    bestMove = -1;
    isSuccessorsUpdated =false;
    resetFeatures();
}

State::State(State* state){
    stboard = new Board(n,m,k,l);
    for(int j=2*n;j>=0;j--){
        for(int i=0;i<2*n+1;i++){
            stboard->config[i][j] = state->stboard->config[i][j];
        }
    }
    heuristic= state->heuristic;
    kInRow = state->kInRow;
    duplicateFeatures(state);
}

void State::resetFeatures() {
    int rowsk1 = -1, rowsk2 = -1;
    int rowskone1 = -1, nonFlipRowskone1 = -1;
    int rowsktwo1 = -1, nonFlipRowsktwo1 = -1;
    int rowskone2 = -1, nonFlipRowskone2 = -1;
    int rowsktwo2 = -1, nonFlipRowsktwo2 = -1;
    int blockDoF1 = -1, blockDoF2 = -1;
}

void State::duplicateFeatures(State* state){

}

void State::getBlockedRings() {
    int p1len = stboard->p1Rings.size();
    int p2len = stboard->p2Rings.size();
    for (int i = 0; i < p1len+p2len; i++) {
        // Ring pair available at index i
        pair<int,int> ring = i>=p1len ? stboard->p2Rings.at(i-p1len) : stboard->p1Rings.at(i);
        int ringx = ring.first, ringy = ring.second;
        // outfile << "Ring: " << ringx << " " << ringy << endl; // Debug
        int blockdof = 0;
        for (int dir = 0; dir < 9; dir++) {
            if (dir==2 || dir==4 || dir==6) continue;
            int xchange = dir/3 - 1;
            int ychange = dir%3 - 1;
            bool limitx = (ringx+xchange >= 0) && (ringx+xchange <= 2*n);
            bool limity = (ringy+ychange >= 0) && (ringy+ychange <= 2*n);
            bool moveAvailable = false, movePossible = false;
            for (int x=ringx+xchange, y=ringy+ychange; limitx && limity; x+=xchange, y+=ychange) {
                movePossible = true;
                if (stboard->config[x][y] == 0) break;
                if (stboard->config[x][y] == 1) {
                    moveAvailable = true;
                    break;
                }
                if (stboard->config[x][y]==2 || stboard->config[x][y]==2) break;
                limitx = (x+xchange >= 0) && (x+xchange <= 2*n);
                limity = (y+ychange >= 0) && (y+ychange <= 2*n);
            }
            if (movePossible && !moveAvailable) {
                blockdof++;
            }
        }
        // outfile << "For ring at " << ringx << " " << ringy << " blocked directions are: " << blockdof << endl; // Debug
        if (i<p1len) this->blockDoF1 += blockdof;
        else this->blockDoF2 += blockdof;
    }
}

void State::setWeight(double weight, int feature) {
    if (feature < weights.size() && feature > 0) {
        weights.at(feature) = weight;
    }
}

void State::incrementkRows(int marker, bool flip, int endx, int endy) {
    if (marker == 4) {
        rowsk1++;
    } else {
        rowsk2++;
    }
    // outfile << "Found streak ending at " << endx << " " << endy << endl;
    endkx = endx;
    endky = endy;
    kInRow = true;
}

void State::incrementRows(int count, int marker, bool flip) {
    // Increments all the in-a-row heuristic variables 
    // for the State object based on the count and the player marker
    if (count == k-2) {
        if (marker == 4) {
            rowsktwo1++;
            if (!flip) nonFlipRowsktwo1++;
        } else {
            rowsktwo2++;
            if (!flip) nonFlipRowsktwo2++;
        } 
    } else if (count == k-1) {
        if (marker == 4) {
            rowskone1++;
            if (!flip) {
                nonFlipRowskone1++;
            }
        } else {
            rowskone2++;
            if (!flip) {
                nonFlipRowskone2++;
            }
        }
    } else {
        // Presently, the code should never get into this -> 
        // k markers checked in getLinearMarkers
        outfile << "Unreachable piece of code reached" << endl;
        incrementkRows(marker, flip, -1, -1);
    }
}

void State::getLinearMarkers() {
    int player1 = 4;
    int player2 = 5;

    // Vertical and Horizontal Rows for both opponents
    for (int i = 0; i <= 2*n; i++) {
        int startj = i>n ? (i-n) : 0;
        int completej = i<=n ? (n+i) : 2*n+1;
        int prevMarkerVert = stboard->config[i][startj];
        int prevMarkerHorz = stboard->config[startj][i];
        int countVert = 1, countHorz = 1;
        // Variables to check if the current streak is non-flippable or not
        bool flipVert = false, flipHorz = false; 
        int j = startj+1;
        int startkxVert = 0, startkyVert = 0;
        int startkxHorz = 0, startkyHorz = 0;
        for (j = startj+1; j < completej; j++) {
            // Vertical Rows
            if (stboard->config[i][j] == prevMarkerVert) {
                if (!(prevMarkerVert == player1 || prevMarkerVert == player2) ) goto horizontal;
                countVert++;
                flipVert = flipVert || stboard->isFlippable(i, j);
            } else {
                if (stboard->config[i][j] == player1 || stboard->config[i][j] == player2) {
                    if (countVert >= k-2 && countVert <= k-1) 
                        incrementRows(countVert, prevMarkerVert, flipVert);
                    if (countVert >= k) {
                        incrementkRows(prevMarkerVert, flipVert, i, j);
                        startkx = startkxVert;
                        startky = startkyVert;
                    }
                    countVert = 1;
                    startkxVert = i;
                    startkyVert = j; // Set the start indices of streak start
                    // outfile << "Start streak at " << i << " " << j << endl; // Debug
                } 
                prevMarkerVert = stboard->config[i][j];
                flipVert = false;
            }
            // outfile << "Checked vertical for " << i << " " << j << endl; // Debug

            // Horizontal Rows
            horizontal:
            if (stboard->config[j][i] == prevMarkerHorz) {
                if (!(prevMarkerHorz == player1 || prevMarkerHorz == player2) ) continue;
                countHorz++;
                flipHorz = flipHorz || stboard->isFlippable(j, i);
            } else {
                if (stboard->config[j][i] == player1 || stboard->config[j][i] == player2) {
                    if (countHorz >= k-2 && countVert <= k-1) 
                        incrementRows(countHorz, prevMarkerHorz, flipHorz);
                    if (countHorz >= k) {
                        incrementkRows(prevMarkerHorz, flipHorz, j, i);
                        startkx = startkxHorz;
                        startky = startkyHorz;
                    }
                    countHorz = 1;
                    startkxHorz = j;
                    startkyHorz = i; // Set the start indices of streak start
                    // outfile << "Start streak at " << j << " " << i << endl; // Debug
                } 
                prevMarkerHorz = stboard->config[j][i];
                flipHorz = false;
            }
            // outfile << "Checked horizontal for " << j << " " << i << endl; // Debug
        }
        
        // Check for the last place
        if (countVert >= k-2 && countVert <= k-1) 
            incrementRows(countVert, prevMarkerVert, flipVert);
        if (countVert >= k) {
            incrementkRows(prevMarkerVert, flipVert, i, j-1);
            startkx = startkxVert;
            startky = startkyVert;
        }

        if (countHorz >= k-2 && countVert <= k-1) 
            incrementRows(countHorz, prevMarkerHorz, flipHorz);
        if (countHorz >= k) {
            incrementkRows(prevMarkerHorz, flipHorz, j-1, i);
            startkx = startkxHorz;
            startky = startkyHorz;
        }
    }

    // Slant Rows for both opponents
    bool flip = false;
    int startkxSlant = 0, startkySlant = 0;
    for (int diff = -n; diff <= n; diff++) {
        // Let the slant line be x-y = c, then diff is the iterator for c
        int startj = (diff<=0) ? 0 : diff;
        int endj = (diff<=0) ? 10 : (10-diff);
        int prevMarker = stboard->config[startj][startj-diff];
        int count = 0;
        int j = startj+1;
        for (j = startj+1; j <= endj; j++) {
            if (stboard->config[j][j-diff] == prevMarker) {
                if (prevMarker != player1 && prevMarker != player2) continue;
                count++;
                flip = flip || stboard->isFlippable(j, j-diff);
            } else {
                if (stboard->config[j][j-diff] == player1 || stboard->config[j][j-diff] == player2) {
                    if (count >= k-2 && count <= k-1) 
                        incrementRows(count, prevMarker, flip);
                    if (count > k) {
                        incrementkRows(prevMarker, flip, j, j-diff);
                        startkx = startkxSlant;
                        startky = startkySlant;
                    } 
                    count = 1;
                    startkxSlant = j;
                    startkySlant = j-diff; // Set the start indices of streak start
                } 
                prevMarker = stboard->config[j][j-diff];
                flip = false;
            }
        }
        if (count >= k-2 && count <= k-1) 
            incrementRows(count, prevMarker, flip);
        if (count > k) {
            incrementkRows(prevMarker, flip, j, j-diff);
            startkx = startkxSlant;
            startky = startkySlant;
        }
    }
}

double State::weightedSum() {
    double h;
    // Rows of k-2 markers
    if (rowsktwo1 != -1) h += rowsktwo1 * weights.at(1);
    if (rowsktwo2 != -1) h += rowsktwo2 * weights.at(2);

    // Rows of k-2 non flippable markers
    if (nonFlipRowsktwo1 != -1) h += nonFlipRowsktwo1 * weights.at(3); 
    if (nonFlipRowsktwo2 != -1) h += nonFlipRowsktwo2 * weights.at(4); 
    
    // Rows of k-1 markers
    if (rowskone1 != -1) h += rowskone1 * weights.at(5);
    if (rowskone2 != -1) h += rowskone2 * weights.at(6);

    // Rows of k-1 non flippable markers
    if (nonFlipRowskone1 != -1) h += nonFlipRowskone1 * weights.at(7);
    if (nonFlipRowskone2 != -1) h += nonFlipRowskone2 * weights.at(8);

    // Rows of k markers
    if (rowsk1 != -1) h += rowsk1 * weights.at(9);
    if (rowsk2 != -1) h += rowsk2 * weights.at(10);

    // Blocked rings
    if (blockDoF1 != -1) h += blockDoF1 * weights.at(11);
    if (blockDoF2 != -1) h += blockDoF2 * weights.at(12);
    return h;
}

/*
 * evaluate() evaluates the current state and checks ->
 * if there exist any row of k markers, in which case, it returns false
 * else returns true 
 */
bool State::evaluate() {
    // Assumption - At a time, only a single row of k markers can be present
    // outfile << "Starting Evaluation" << endl; // Debug
    stboard->updateRingPositions();
    this->getLinearMarkers();
    if (kInRow) {
        return false;
    }
    this->getBlockedRings();
    double h = weightedSum();
    heuristic = h;
    return true;
}

double State::getEvaluation() {
    return heuristic;
}

bool State::isTerminalNode() {
    // Assumption -> It shall never happen that 
    // one player has no available moves while the other player has
    bool retVal = false;
    stboard->updateRingPositions();
    if (stboard->p1Rings.size() == m-l || stboard->p2Rings.size() == m-l) {
        retVal = true;
    } else {
        retVal = true;
        for (pair<int,int> ring : stboard->p1Rings) {
            vector<pair<int,int>> moves = stboard->showPossibleMoves(ring.first, ring.second);
            if (moves.size() != 0) {
                retVal = false;
                break;
            }
        }
    }
    return retVal;
}

double State::iterativeDeepening(int max_depth, int playerId){
    double val;
    // outfile << "ID starting for depth " << max_depth << endl;
    for(int distance = 1; distance < max_depth && !timeHelper->outOfTime(); distance++) {
        outfile << "ID evaluating for depth " << distance << endl;
        val = this->alphaBeta(distance,-DBL_MAX, DBL_MAX, playerId);
    }
    outfile << "ID Done for this move, found successor at " << this->bestMove << endl;
    return val;
}

double State::alphaBeta(int depth, double alpha, double beta, int currPlayer){
    if(depth==0 || this->isTerminalNode()){
        return this->getEvaluation();
    }
    outfile << "executing alphabeta at depth "<<depth << " alpha is "<<alpha << " beta is "<< beta << " player is "<<currPlayer<<endl;
    outfile << "state is "<<endl;
    this->stboard->printnormalconfig();
    outfile << endl;

    double tempscore = -DBL_MAX;
    vector<State *> successsors = this->getSuccessors(currPlayer);
    for(int i=0;i<successsors.size() && !timeHelper->outOfTime();i++){
        double value = -successsors[i]->alphaBeta(depth-1,-beta,-alpha, 3-currPlayer);
        if(value>tempscore){
            outfile << "Found better successor at index " << i << endl;
            this->bestMove = i;
            tempscore=value;
        }
        if(tempscore>alpha){
            alpha=tempscore;
        }
        if(tempscore>=beta){
            break;
        }
    }
    outfile << "returning alphabeta at depth "<<depth << " alpha is "<<alpha << " beta is "<< beta << " player is "<<currPlayer<<endl;
    outfile << "state is "<<endl;
    this->stboard->printnormalconfig();
    outfile <<endl;
    return tempscore;
}

vector<State*> State::getSuccessors(int currPlayer){
    if(this->isSuccessorsUpdated){
        if(this->bestMove>0 ){
            iter_swap(this->successors.begin(),this->successors.begin()+this->bestMove );
            iter_swap(this->moves.begin(), this->moves.begin()+this->bestMove);
            this->bestMove=0;
        }
        outfile<<"returning get successor"<<endl;
        return this->successors;
    }
    bool isKinRow = this->evaluate();
    // outfile << "evaluation for the state done "<<isKinRow<<endl;
    // outfile << "myass "<< true<<endl;

    vector<State*> movedStates, finStatesvec;
    vector<string> movedMoves, finStatesMoves;

    if(!isKinRow) {
        //there are k in row we need to remove them
        vector<State*> tempvec; 
        vector<string> tempmoves;
        outfile<<"removing markers case, subproblem 1 "<<endl;
        vector<pair< pair<int,int>, pair<int,int>>> removalCoordinates = this->getPossibleMarkerRemovals();
        for(auto removaliter= removalCoordinates.begin();removaliter<removalCoordinates.end();removaliter++){
            /* 
            * For Debug -> Take care to change code in subproblem 3 as well
            */
            State * changedstate = new State(this->stboard);
            int startx = removaliter->first.first, starty = removaliter->first.second;
            int endx = removaliter->second.first, endy = removaliter->second.second;
            changedstate->stboard->removeMarkers(startx, starty, endx, endy);
            string tempMove = parseMove(2, startx, starty, endx, endy);
            // removed markers
            vector<pair<int,int>> remrings = currPlayer==1 ? changedstate->stboard->p1Rings : changedstate->stboard->p2Rings;

            for(int ringsiter=0;ringsiter<remrings.size();ringsiter++){
                State * removedRingState = new State(changedstate->stboard);
                int ringx = remrings[ringsiter].first, ringy = remrings[ringsiter].second;
                removedRingState->stboard->removeRing(ringx, ringy);
                removedRingState->stboard->updateRingPositions();
                
                tempvec.push_back(removedRingState);
                tempmoves.push_back(tempMove + " " + parseMove(3, ringx, ringy, -1, -1));
            }
        }
        
        outfile <<"removed markers and rings, subproblem 1 done, jumping to subproblem 2"<<endl;
        for(int tempiter=0;tempiter<tempvec.size();tempiter++){
            auto movePair = tempvec[tempiter]->getStatesForMoves(currPlayer, tempmoves[tempiter]);
            vector<State*> tempthisMovedStates = movePair.first;
            vector<string> tempthisMoves = movePair.second;
            movedStates.insert(movedStates.end(), tempthisMovedStates.begin(), tempthisMovedStates.end());
            movedMoves.insert(movedMoves.end(), tempthisMoves.begin(), tempthisMoves.end());
        }
    } else {
        outfile << "skipped subproblem 1, calculating subproblem 2"<<endl;
        auto movePair = this->getStatesForMoves(currPlayer, "\0");
        movedStates = movePair.first;
        movedMoves = movePair.second;
    }

    // Final step of checking if k markers made again
    outfile << "subproblem 2 is done, now lets move to subproblem 3 if there or not, it will be checked "<< movedStates.size()<< " times"<<endl; // Debug

    for(int iterMovedStates=0; iterMovedStates<movedStates.size(); iterMovedStates++){
        State * thismovedstate = movedStates[iterMovedStates];
        string appendMove = movedMoves[iterMovedStates];
        // outfile << "thismovedstate is "<<endl;
        // thismovedstate->stboard->printnormalconfig(); // Debug
        isKinRow = thismovedstate->evaluate();
        // outfile<<"debug"<<endl;
        /* 
         * For Debug -> Take care to change code in subproblem 1 as well
         */
        if(!isKinRow){
            outfile << "execute subproblem 3 as it is necessary"<<endl;
            vector<pair< pair<int,int>, pair<int,int>>> removalCoordinates = thismovedstate->getPossibleMarkerRemovals();
            outfile << "removalcoordinates freq is "<< removalCoordinates.size()<<endl;
            // remove trashcount
            int trashcount =1;
            for(auto removaliter= removalCoordinates.begin();removaliter<removalCoordinates.end();removaliter++){
                outfile << "iterating on removalcoordinates number "<< trashcount<<endl;
                State * changedstate = new State(thismovedstate->stboard);
                int startx = removaliter->first.first, starty = removaliter->first.second;
                int endx = removaliter->second.first, endy = removaliter->second.second;
                changedstate->stboard->removeMarkers(startx, starty, endx, endy);
                string tempmove = parseMove(2, startx, starty, endx, endy);
                // removed markers
                // outfile << "removed rings state is "<<endl;
                // changedstate->stboard->printnormalconfig(); // Debug

                vector<pair<int,int>> remrings = currPlayer==1 ? changedstate->stboard->p1Rings : changedstate->stboard->p2Rings;
                for(int ringsiter=0; ringsiter < remrings.size(); ringsiter++){
                    State * removedRingState = new State(changedstate->stboard);
                    int ringx = remrings[ringsiter].first, ringy = remrings[ringsiter].second;
                    removedRingState->stboard->removeRing(ringx, ringy);
                    removedRingState->stboard->updateRingPositions();                    
                    finStatesvec.push_back(removedRingState);
                    finStatesMoves.push_back(appendMove + " " + tempmove + " " + parseMove(3, ringx, ringy, -1, -1));
                }
                trashcount++;
            }
        } else {
            // outfile << "skipped subproblem three as not true"<<endl;
            finStatesvec.push_back(thismovedstate);
            finStatesMoves.push_back(appendMove);
            // outfile << "pushed"<<endl;
        }
    }
    
    this->isSuccessorsUpdated = true;
    this->successors = finStatesvec;
    this->moves = finStatesMoves;
    outfile<<"returning get successor"<<endl;
    return finStatesvec;
}

pair<vector<State*>, vector<string>> State::getStatesForMoves(int currPlayer, string appendMove){
    vector<State*> ansvec;
    vector<string> ansmoves;
    vector<pair<int,int>> allRings = currPlayer==1 ? stboard->p1Rings : stboard->p2Rings; 
    
    for(int iterring=0; iterring<allRings.size(); iterring++){
        auto thisring = allRings[iterring];
        auto possibleMoves = this->stboard->showPossibleMoves(thisring.first,thisring.second);
        for(int individualmoveiter=0; individualmoveiter<possibleMoves.size(); individualmoveiter++){
            auto thismovefin = possibleMoves[individualmoveiter];
            State * tempstate = new State(this->stboard);
            tempstate->stboard->selectAndMoveRing(thisring.first, thisring.second, thismovefin.first, thismovefin.second);
            string move = parseMove(1, thisring.first, thisring.second, thismovefin.first, thismovefin.second);

            ansvec.push_back(tempstate);
            ansmoves.push_back(appendMove + " " + move);
        }
    }
    return make_pair(ansvec,ansmoves);
}

vector<pair< pair<int,int>, pair<int,int>>> State::getPossibleMarkerRemovals(){
    vector<pair< pair<int,int>, pair<int,int>>> ansvec;
    outfile << "marker coordinates are "<< this->startkx << " " <<this->startky<< " " << this->endkx<< " " <<this->endky<< " "<< endl;
    int distance = max(abs(this->startkx-this->endkx)+1,abs(this->endkx-this->endky)+1);
    if(distance==k){
        ansvec.push_back(make_pair(make_pair(this->startkx,this->startky),make_pair(this->endkx, this->endky)));
    }else{
        auto dirvec = this->stboard->getDirectionVector(make_pair(this->startkx,this->startky), make_pair(this->endkx, this->endky));
        ansvec.push_back(make_pair(make_pair(this->startkx,this->startky),make_pair(this->startkx + k*dirvec.first, this->startky+k*dirvec.second)));
        ansvec.push_back(make_pair(make_pair(this->endkx-k*dirvec.first, this->endky-k*dirvec.second),make_pair(this->endkx, this->endky)));
    }
    return ansvec;
}

void State::makeMove() {
    State* nextState = this->successors.at(bestMove);
    board = nextState->stboard;
}
