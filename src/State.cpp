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

void changePlayer(vector<pair<State*,string> > preFinal) {
    for (int i = 0; i < preFinal.size(); i++) {
        preFinal.at(i).first->playerToMove = 3-preFinal.at(i).first->playerToMove;
    }
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

State::State(Board* board, int player) {
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
    heuristic = -DBLMAX;
    kInRow = false;

    playerToMove = player;
    bestMove = -1;
    isSuccessorsUpdated = false;
    resetFeatures();
}

// State::State(State* state){
//     stboard = new Board(n,m,k,l);
//     for(int j=2*n;j>=0;j--){
//         for(int i=0;i<2*n+1;i++){
//             stboard->config[i][j] = state->stboard->config[i][j];
//         }
//     }
//     heuristic= state->heuristic;
//     kInRow = state->kInRow;
//     duplicateFeatures(state);
// }

void State::resetFeatures() {
    rowsk1 = -1; rowsk2 = -1;
    rowskone1 = -1; nonFlipRowskone1 = -1;
    rowsktwo1 = -1; nonFlipRowsktwo1 = -1;
    rowskone2 = -1; nonFlipRowskone2 = -1;
    rowsktwo2 = -1; nonFlipRowsktwo2 = -1;
    blockDoF1 = -1; blockDoF2 = -1;
}

void State::getBlockedRings() {
    int p1len = stboard->p1Rings.size();
    int p2len = stboard->p2Rings.size();
    bool p1blocked = true, p2blocked = true;
    for (int i = 0; i < p1len+p2len; i++) {
        // Ring pair available at index i
        pair<int,int> ring = i>=p1len ? stboard->p2Rings.at(i-p1len) : stboard->p1Rings.at(i);
        int ringx = ring.first, ringy = ring.second;
        // outfile << "Ring: " << ringx << " " << ringy << endl; // Debug
        int blockdof = 0;
        bool allDirBlocked = true;
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
                    allDirBlocked = false;
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
        if (i<p1len) {
            p1blocked = p1blocked && allDirBlocked;
            this->blockDoF1 += blockdof;
        }
        else {
            p2blocked = p2blocked && allDirBlocked;
            this->blockDoF2 += blockdof;
        }
    }
    this->rings1Blocked = p1blocked;
    this->rings2Blocked = p2blocked;
}

void State::setWeight(double weight, int feature) {
    if (feature < weights.size() && feature > 0) {
        weights.at(feature) = weight;
    }
}

void State::incrementkRows(int marker, bool flip, int endx, int endy) {
    if (marker == 4) {
        rowsk1++;
        if (DEBUG_EVAL) outfile << "Found k for 1, rowsk1: " << rowsk1 << endl;
    } else {
        if (DEBUG_EVAL) outfile << "rowsk2: " << rowsk2 << endl;
        rowsk2++;
        if (DEBUG_EVAL) outfile << "Found k for 2, rowsk2: " << rowsk2 << endl;
    }
    if (marker-playerToMove == 3) {
        if (WRITE_FILE) outfile << "Found streak end: " << endx << " " << endy << " for player " << playerToMove << " in state " << endl;
        if (DEBUG_EVAL) {
            this->stboard->printnormalconfig();
            if (WRITE_FILE) outfile << endl;
        }
        endkx = endx;
        endky = endy;
        kInRow = true;
    }
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
        if (WRITE_FILE) outfile << "Unreachable piece of code reached" << endl;
        incrementkRows(marker, flip, -1, -1);
    }
}

void State::checkCount(int count, int prevMarker, bool flip, int startx, int starty, int endx, int endy) {
    if (count >= k-2 && count <= k-1) 
        incrementRows(count, prevMarker, flip);
    if (count >= k) {
        incrementkRows(prevMarker, flip, endx, endy);
        if (WRITE_FILE) outfile << "Found streak starting at: " << startx << " " << starty << endl;
        if (prevMarker-playerToMove == 3) {
            // outfile << "For same player" << endl;
            startkx = startx;
            startky = starty;
        }
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
        int startkxVert = i, startkyVert = startj;
        int startkxHorz = startj, startkyHorz = i;
        if (DEBUG_EVAL) outfile << "Checking vertical at " << i << " from " << startj << " to " << completej << endl; 
        for (j = startj+1; j < completej; j++) {
            // Vertical Rows
            if (stboard->config[i][j] == prevMarkerVert) {
                if (!(prevMarkerVert == player1 || prevMarkerVert == player2) ) goto horizontal;
                countVert++;
                if (NON_FLIP) flipVert = flipVert || stboard->isFlippable(i, j);
            } else {
                if (countVert >= k-2) {
                    checkCount(countVert, prevMarkerVert, flipVert, startkxVert, startkyVert, i, j-1);
                } 
                startkxVert = i;
                startkyVert = j; // Set the start indices of streak start
                if (DEBUG_EVAL) outfile << "Start streak (Vert) at " << i << " " << j << endl; // Debug
                countVert = 1;
                prevMarkerVert = stboard->config[i][j];
                flipVert = false;
            }
            // outfile << "Checked vertical for " << i << " " << j << endl; // Debug

            // Horizontal Rows
            horizontal:
            if (stboard->config[j][i] == prevMarkerHorz) {
                if (!(prevMarkerHorz == player1 || prevMarkerHorz == player2) ) continue;
                countHorz++;
                if (NON_FLIP) flipHorz = flipHorz || stboard->isFlippable(j, i);
            } else {
                if (countHorz > 0) {
                    checkCount(countHorz, prevMarkerHorz, flipHorz, startkxHorz, startkyHorz, j-1, i);
                } 
                startkxHorz = j;
                startkyHorz = i; // Set the start indices of streak start
                if (DEBUG_EVAL) outfile << "Start streak (Horz) at " << j << " " << i << endl; // Debug
                countHorz = 1;
                prevMarkerHorz = stboard->config[j][i];
                flipHorz = false;
            }
            // outfile << "Checked horizontal for " << j << " " << i << endl; // Debug
        }
        
        // Check for the last place
        if (countVert >= k) {
            checkCount(countVert, prevMarkerVert, flipVert, startkxVert, startkyVert, i, j-1);
        }
        if (countHorz >= k) {
            checkCount(countHorz, prevMarkerHorz, flipHorz, startkxHorz, startkyHorz, j-1, i);
        }
    }

    // Slant Rows for both opponents
    bool flip = false;
    for (int diff = -n; diff <= n; diff++) {
        // Let the slant line be x-y = c, then diff is the iterator for c
        int startj = (diff<=0) ? 0 : diff;
        int completej = (diff<=0) ? (10+diff) : 10;
        int prevMarker = stboard->config[startj][startj-diff];
        int count = 1;
        int j = startj+1;
        int startkxSlant = startj, startkySlant = startj-diff;
        if (DEBUG_EVAL) outfile << "Checking slant rows from: " << startj << "," << startj-diff << " to " << completej << "," << completej-diff << endl;
        for (j = startj+1; j <= completej; j++) {
            if (stboard->config[j][j-diff] == prevMarker) {
                if (prevMarker != player1 && prevMarker != player2) continue;
                count++;
                if (NON_FLIP) flip = flip || stboard->isFlippable(j, j-diff);
            } else {
                if (count >= k-2) {
                    if (DEBUG_EVAL) outfile << "Found a streak (Slant) of length " << count << endl;
                    checkCount(count, prevMarker, flip, startkxSlant, startkySlant, j-1, j-1-diff); 
                }
                startkxSlant = j;
                startkySlant = j-diff; // Set the start indices of streak start
                if (DEBUG_EVAL) outfile << "Streak start (Slant) at " << j << " " << (j-diff) << endl;
                count = 1;
                prevMarker = stboard->config[j][j-diff];
                flip = false;
            }
        }
        if (count >= k-2) {
            checkCount(count, prevMarker, flip, startkxSlant, startkySlant, j-1, j-1-diff);
        }
    }
}

double State::weightedSum() {
    double h;
    // Rows of k-2 markers
    if (rowsktwo1 != -1) {
        if (DEBUG_EVAL) outfile << "Rows k-2 for player 1 " << rowsktwo1 << endl;
        h += rowsktwo1 * weights.at(1);
    } 
    if (rowsktwo2 != -1) {
        if (DEBUG_EVAL) outfile << "Rows k-2 for player 2 " << rowsktwo1 << endl;
        h += rowsktwo2 * weights.at(2);
    }

    if (NON_FLIP) {
        // Rows of k-2 non flippable markers
        if (nonFlipRowsktwo1 != -1) {
            if (DEBUG_EVAL) outfile << "Rows k-2 for player 1 - non flip " << nonFlipRowsktwo1 << endl;        
            h += nonFlipRowsktwo1 * weights.at(3); 
        }
        if (nonFlipRowsktwo2 != -1) {
            if (DEBUG_EVAL) outfile << "Rows k-2 for player 2 -  non flip " << nonFlipRowsktwo2 << endl;    
            h += nonFlipRowsktwo2 * weights.at(4); 
        }
    }

    // Rows of k-1 markers
    if (rowskone1 != -1) {
        if (DEBUG_EVAL) outfile << "Rows k-1 for player 1 " << rowskone1 << endl;
        h += rowskone1 * weights.at(5);
    }
    if (rowskone2 != -1) {
        if (DEBUG_EVAL) outfile << "Rows k-1 for player 2 " << rowskone2 << endl;
        h += rowskone2 * weights.at(6);
    }

    if (NON_FLIP) {
        // Rows of k-1 non flippable markers
        if (nonFlipRowskone1 != -1) {
            if (DEBUG_EVAL) outfile << "Rows k-1 for player 1 - non flip " << nonFlipRowskone1 << endl;        
            h += nonFlipRowskone1 * weights.at(7);
        }
        if (nonFlipRowskone2 != -1) {
            if (DEBUG_EVAL) outfile << "Rows k-1 for player 2 - non flip " << nonFlipRowskone2 << endl;
            h += nonFlipRowskone2 * weights.at(8);
        }
    }

    // Rows of k markers
    if (rowsk1 != -1) {
        if (DEBUG_EVAL) outfile << "Rows k for player 1 " << rowsk1 << endl;
        h += rowsk1 * weights.at(9);
    }
    if (rowsk2 != -1) {
        if (DEBUG_EVAL) outfile << "Rows k for player 2 " << rowsk2 << endl;
        h += rowsk2 * weights.at(10);
    }

    // Blocked rings
    if (blockDoF1 != -1) {
        if (DEBUG_EVAL) outfile << "Blocked DoF for player 1 " << blockDoF1 << endl;
        h += blockDoF1 * weights.at(12);
    }
    if (blockDoF2 != -1) {
        if (DEBUG_EVAL) outfile << "Blocked DoF for player 2 " << blockDoF2 << endl;
        h += blockDoF2 * weights.at(11);
    }

    // Number of rings removed
    // if (player_id == 1) {
    //     h += (m - stboard->p1Rings.size()) * weights.at(13);
    //     h += (m - stboard->p2Rings.size()) * weights.at(14);
    // } else {
    h += (m - stboard->p1Rings.size()) * weights.at(13);
    h += (m - stboard->p2Rings.size()) * weights.at(14);
    if (DEBUG_EVAL) {
        outfile << "Player 1 removed rings:- " << weights.at(13) << endl;
        outfile << "Player 2 removed rings:- " << weights.at(14) << endl;
    }
    // }
    return h;
}

/*
 * getEvaluation() evaluates the current state and checks ->
 * if there exist any row of k markers, in which case, it returns false
 * else returns true 
 */
bool State::evaluate() {
    // Assumption - At a time, only a single row of k markers can be present
    if (DEBUG_EVAL) outfile << "Starting Evaluation" << endl; // Debug
    stboard->updateRingPositions();
    this->getLinearMarkers();
    if (kInRow) {
        return false;
    }
    this->getBlockedRings();
    if (DEBUG_EVAL) outfile << "Features done" << endl;
    double h = this->weightedSum();
    heuristic = h;
    return true;
}

double State::getEvaluation() {
    if (heuristic == -DBLMAX) {
        bool check = this->evaluate();
        // if (!check) outfile << "WARNING: Invalid state evaluated" << endl; // getEvaluation() being called everywhere
    }
    // outfile << "Evaluation done: " << heuristic << endl; 
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
        this->getBlockedRings();
        retVal = rings1Blocked || rings2Blocked;
    }
    if (retVal) {
        if (WRITE_FILE) outfile << "~~~~~~~~~~~Found Terminal Node~~~~~~~~~~~~" << endl;
        if (WRITE_FILE) this->stboard->printnormalconfig();
        if (WRITE_FILE) outfile << endl;
        if (WRITE_FILE) outfile << "   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   " << endl;
    }
    return retVal;
}

double State::iterativeDeepening(int max_depth, int playerId){
    double val;
    // outfile.close();
    // outfile.open("console.log");
    // outfile << "ID starting for depth " << max_depth << endl;
    for(int distance = 1; distance <= max_depth && !timeHelper->outOfTime(); distance++) {
        if (WRITE_FILE) outfile << "ID evaluating for depth " << distance << endl;
        val = this->alphaBeta(distance,-DBLMAX, DBLMAX, playerId, 1);
        outfile << "best move at distance "<<distance << " is "<< this->bestMove<<endl;
    }
    if (WRITE_FILE) {
        outfile << "ID Done for this move, found successor at: " << this->bestMove << endl;
        this->successors.at(bestMove).first->stboard->printnormalconfig();
        outfile << endl; // Debug
    } 
    return val;
}

double State::alphaBeta(int depth, double alpha, double beta, int currPlayer, int evSign){
    initempalpha=alpha;
    initempbeta=beta;
    if(depth==0){
        alphaBetaValue = evSign * this->getEvaluation();
        return evSign * this->getEvaluation();
    }
    if (this->isTerminalNode()) {
        return evSign * this->getEvaluation();
    }    
    if (WRITE_FILE) {
        outfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        outfile << "executing alphabeta at depth "<<depth << " alpha is "<<alpha << " beta is "<< beta << " player is "<<currPlayer<< " ";
        outfile << "state is "<<endl;
        this->stboard->printnormalconfig();
        outfile << endl;
    } 

    double tempscore = -DBLMAX;
    this->getSuccessors(currPlayer);

    // Sort the successors
    sort(successors.begin(), successors.end(), [currPlayer](const pair<State*,string>& p1, const pair<State*,string>& p2) -> bool {
        if (p1.first == NULL || p2.first == NULL)
            return true;
        if (currPlayer == player_id)
            return p1.first->getEvaluation() > p2.first->getEvaluation();
        return p1.first->getEvaluation() < p2.first->getEvaluation();
    });
    this->bestMove = 0;

    /*
     * Not much effect in the overall timing of gameplay was observed when
     * the nodes were ordered, thereby implying that the best move (zero index) is quite good in most cases
     * Hence, Pruning the worst ones, can be a really good option
     * 
     * CHECK -> WHETHER THIS LOCATION FOR ERASING THE VECTOR IS FINE OR NOT
     */
    int len = successors.size();
    int beam = successors.size() < 15 ? successors.size() : 15;
    successors.erase(successors.begin()+beam, successors.end());
    // outfile << "Pruned successors length: " << successors.size() << endl;

    for(int i = 0; i < successors.size() && !timeHelper->outOfTime(); i++){
        double value = -successors[i].first->alphaBeta(depth-1,-beta,-alpha, 3-currPlayer, -evSign);
        // double temp = this->successors[i].first->getEvaluation();
        if (WRITE_FILE) outfile << value << " " << this->successors[i].first->getEvaluation() << endl;
        if (WRITE_FILE) {
            this->successors[i].first->stboard->printnormalconfig();
            outfile << endl;
        }
        if(value>tempscore) {
            // double temp = this->successors[i].first->getEvaluation();
            if (WRITE_FILE) outfile << "    Better -> " << successors.at(i).first->getEvaluation() << endl;
            this->bestMove = i;
            tempscore=value;
        }
        if(tempscore > alpha) {
            alpha = tempscore;
        }
        if(alpha >= beta) {
            // outfile << "Pruning done at " << i << " at depth " << depth << endl; 
            break;
        }
        if (WRITE_FILE) outfile << endl;
    }
    if (WRITE_FILE) outfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    alphaBetaValue=tempscore;
    fintempalpha=alpha;
    fintempbeta = beta;
    return tempscore;
}

vector<pair<State*,string> > State::getSuccessors(int currPlayer){
    if(this->isSuccessorsUpdated){
        if(this->bestMove>0 ){
            iter_swap(this->successors.begin(), this->successors.begin()+this->bestMove);
            // iter_swap(this->moves.begin(), this->moves.begin()+this->bestMove);
            this->bestMove=0;
        }
        if (WRITE_FILE) outfile<<"returning get successor"<<endl;
        return this->successors;
    }
    this->getEvaluation();
    bool isKinRow = this->kInRow;
    // bool isKinRow = this->evaluate();
    // outfile << "evaluation for the state done "<<isKinRow<<endl;

    vector<pair<State*,string> > movedStates, finStatesvec;

    if(isKinRow) {
        //there are k in row we need to remove them
        vector<pair<State*,string> > tempvec; 
        outfile<<"removing markers case, subproblem 1 "<<endl;
        vector<pair< pair<int,int>, pair<int,int>>> removalCoordinates = this->getPossibleMarkerRemovals();
        for(auto removaliter= removalCoordinates.begin();removaliter<removalCoordinates.end();removaliter++){
            /* 
            * For Debug -> Take care to change code in subproblem 3 as well
            */
            State * changedstate = new State(this->stboard, playerToMove);
            int startx = removaliter->first.first, starty = removaliter->first.second;
            int endx = removaliter->second.first, endy = removaliter->second.second;
            changedstate->stboard->removeMarkers(startx, starty, endx, endy);
            string tempMove = parseMove(2, startx, starty, endx, endy);
            // removed markers
            vector<pair<int,int>> remrings = currPlayer==1 ? changedstate->stboard->p1Rings : changedstate->stboard->p2Rings;

            for(int ringsiter=0;ringsiter<remrings.size();ringsiter++){
                State * removedRingState = new State(changedstate->stboard, playerToMove);
                int ringx = remrings[ringsiter].first, ringy = remrings[ringsiter].second;
                removedRingState->stboard->removeRing(ringx, ringy);
                removedRingState->stboard->updateRingPositions();
                auto tempPair = make_pair(removedRingState, tempMove + " " + parseMove(3, ringx, ringy, -1, -1));
                tempvec.push_back(tempPair);
            }
        }
        vector<pair<int,int>> rings = currPlayer == 1 ? this->stboard->p1Rings : this->stboard->p2Rings;
        if (rings.size() != m-l+1) {
            if (WRITE_FILE) outfile <<"removed markers and rings, subproblem 1 done, jumping to subproblem 2"<<endl;
            for (int tempiter = 0; tempiter < tempvec.size(); tempiter++){
                auto movePair = tempvec[tempiter].first->getStatesForMoves(currPlayer, tempvec[tempiter].second);
                movedStates.insert(movedStates.end(), movePair.begin(), movePair.end());
            }
        } else {
            // GAME FINISHED
            movedStates.insert(movedStates.end(), tempvec.begin(), tempvec.end());

            this->isSuccessorsUpdated = true;
            changePlayer(movedStates);
            this->successors = movedStates;
            if (WRITE_FILE) outfile<<"returning get successor"<<endl;
            return movedStates;
        }
    } else {
        if (WRITE_FILE) outfile << "skipped subproblem 1, calculating subproblem 2"<<endl;
        movedStates = this->getStatesForMoves(currPlayer, "\0");
    }

    // Final step of checking if k markers made again
    if (WRITE_FILE) outfile << "subproblem 2 is done, now lets move to subproblem 3 if there or not, it will be checked "<< movedStates.size()<< " times"<<endl; // Debug

    for(int iterMovedStates=0; iterMovedStates<movedStates.size(); iterMovedStates++){
        State * thismovedstate = movedStates[iterMovedStates].first;
        string appendMove = movedStates[iterMovedStates].second;

        thismovedstate->getEvaluation();
        bool isKinRow = thismovedstate->kInRow;
        // bool isKinRow = thismovedstate->evaluate();
        /* 
         * For Debug -> Take care to change code in subproblem 1 as well
         */
        if(isKinRow){
            if (WRITE_FILE) outfile << "execute subproblem 3 as it is necessary"<<endl;
            vector<pair< pair<int,int>, pair<int,int>>> removalCoordinates = thismovedstate->getPossibleMarkerRemovals();
            if (WRITE_FILE) outfile << "removalcoordinates freq is "<< removalCoordinates.size()<<endl;
            // remove trashcount
            int trashcount =1;
            for(auto removaliter= removalCoordinates.begin();removaliter<removalCoordinates.end();removaliter++){
                if (WRITE_FILE) outfile << "iterating on removalcoordinates number "<< trashcount<<endl;
                State * changedstate = new State(thismovedstate->stboard, playerToMove);
                int startx = removaliter->first.first, starty = removaliter->first.second;
                int endx = removaliter->second.first, endy = removaliter->second.second;
                changedstate->stboard->removeMarkers(startx, starty, endx, endy);
                string tempmove = parseMove(2, startx, starty, endx, endy);
                // removed markers
                // outfile << "removed rings state is "<<endl;
                // changedstate->stboard->printnormalconfig(); // Debug

                vector<pair<int,int>> remrings = currPlayer==1 ? changedstate->stboard->p1Rings : changedstate->stboard->p2Rings;
                for(int ringsiter=0; ringsiter < remrings.size(); ringsiter++){
                    State * removedRingState = new State(changedstate->stboard, playerToMove);
                    int ringx = remrings[ringsiter].first, ringy = remrings[ringsiter].second;
                    removedRingState->stboard->removeRing(ringx, ringy);
                    removedRingState->stboard->updateRingPositions();   
                    auto finPair = make_pair(removedRingState, appendMove + " " + tempmove + " " + parseMove(3, ringx, ringy, -1, -1));     
                    finStatesvec.push_back(finPair);
                }
                trashcount++;
            }
        } else {
            // outfile << "skipped subproblem three as not true"<<endl;
            finStatesvec.push_back(make_pair(thismovedstate,appendMove));
        }
    }
    
    this->isSuccessorsUpdated = true;
    changePlayer(finStatesvec);
    this->successors = finStatesvec;
    if (WRITE_FILE) outfile<<"returning get successor"<<endl;
    return finStatesvec;
}

vector<pair<State*,string> > State::getStatesForMoves(int currPlayer, string appendMove){
    vector<pair<State*,string> > ansvec;
    vector<pair<int,int>> allRings = currPlayer==1 ? stboard->p1Rings : stboard->p2Rings; 
    
    for(int iterring=0; iterring<allRings.size(); iterring++){
        auto thisring = allRings[iterring];
        auto possibleMoves = this->stboard->showPossibleMoves(thisring.first,thisring.second);
        for(int individualmoveiter=0; individualmoveiter<possibleMoves.size(); individualmoveiter++){
            auto thismovefin = possibleMoves[individualmoveiter];
            State * tempstate = new State(this->stboard, playerToMove);
            tempstate->stboard->selectAndMoveRing(thisring.first, thisring.second, thismovefin.first, thismovefin.second);
            string move = parseMove(1, thisring.first, thisring.second, thismovefin.first, thismovefin.second);

            ansvec.push_back(make_pair(tempstate,appendMove + " " + move));
        }
    }
    return ansvec;
}

vector<pair< pair<int,int>, pair<int,int>>> State::getPossibleMarkerRemovals(){
    vector<pair< pair<int,int>, pair<int,int>>> ansvec;
    if (WRITE_FILE) outfile << "marker coordinates are "<< this->startkx << " " <<this->startky<< " " << this->endkx<< " " <<this->endky<< " "<< endl;
    int distance = max( abs(this->startkx-this->endkx)+1, abs(this->startky-this->endky)+1);
    if(distance==k){
        ansvec.push_back(make_pair(make_pair(this->startkx,this->startky),make_pair(this->endkx, this->endky)));
    }else{
        auto dirvec = this->stboard->getDirectionVector(make_pair(this->startkx,this->startky), make_pair(this->endkx, this->endky));
        ansvec.push_back(make_pair(make_pair(this->startkx,this->startky),make_pair(this->startkx + (k-1)*dirvec.first, this->startky+(k-1)*dirvec.second)));
        ansvec.push_back(make_pair(make_pair(this->endkx-(k-1)*dirvec.first, this->endky-(k-1)*dirvec.second),make_pair(this->endkx, this->endky)));
    }
    return ansvec;
}

void State::makeMove() {
    State* nextState = this->successors.at(bestMove).first;
    board = nextState->stboard;
}

void State::mydebug(){
    outfileShaved << "my inialpha, inibeta, finalpha, finbeta is "<< initempalpha<<" "<<initempbeta << " " << fintempalpha << " "<< fintempbeta<<endl;
    outfileShaved << "alphabeta values are "<<endl;
    for(int i=0;i<successors.size();i++){
        outfileShaved << successors[i].first->alphaBetaValue << " "; 
    }
    outfileShaved << endl << "bestMoves values are "<<endl;
    for(int i=0;i<successors.size();i++){
        outfileShaved << successors[i].first->bestMove << " "; 
    }
    outfileShaved << endl;
}

void State::mydebugdepth(int depthm){
    outfileShaved << "depth is "<<depthm<<endl;
    if(depthm==0){
        return;
    }
    this->mydebug();
    outfileShaved << "time for children "<<endl;
    for(int i=0;i<successors.size();i++){
        successors[i].first->mydebug();
        outfileShaved << "next child please "<<endl;
    }
    outfileShaved << "all childs complete of depth "<<depthm<<endl;
}