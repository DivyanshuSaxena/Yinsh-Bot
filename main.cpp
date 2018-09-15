/* 
 * File:   Board.cpp
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
/**
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * WARNING:  Do not remove the comment below.
 *          - The comment is added for CODE SECURITY AND PROTECTION
 *          - It also facilitates proper working of the code
 *          - If you remove this comment, the code might malfunction
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *				                   i`
 *				                  ;@@
 *				                ~@@@@
 *				                 @@@O   ^*
 *				                  @&   o@@-
 *				              &&.    _@@@@@
 *				               @@@@@@@@@@@@
 *				     !@@@@@?   ~@@@@@@@@@#
 *				   O@@@@@@@@@    &@@@@@#^
 *				 `@@@@@@@@@@@@     -_`
 *				 +@@@@@&*#@@@@:       &@@@@e
 *				  @@&`    `@@@&      @@@@@@@&
 *				           &@@&     i@@@@@@@@?
 *				           @@@@     @@@@@@@@@@
 *				          -@@@&    %@@@@&O@@@@?
 *				         o@@@@    ^@@@@+   &@@@
 *				       %@@@@@~    @@@@#     @@@
 *				      %@@@@@`   _@@@@@      &@@.
 *				      %@@@@&^_%@@@@@@       +@@_
 *				      i@@@@@@@@@@@@#        ;@@o
 *				      .@@@@@@@@@@@~         +@@$
 *				       :&@@@@@@#.           #@@!
 *				          %@@@@;            @@@:
 *   		   e            O@@@!           +@@@.
 *  		   ^&            @@@@   !      `@@@@
 *				@O           @@@@   ;@    _@@@@&
 *				O@#          @@@@   `@@&#@@@@@@_
 *				`@@@`       e@@@&    @@@@@@@@@@
 *				 #@@@#     .@@@@*    !@@@@@@@@
 *				  &@@@@@@&@@@@@@      @@@@@@@
 *				  -@@@@@@@@@@@@@       :$#!`
 *				   _@@@@@@@@@@@
 *				     #@@@@@@@&
 *				      -?@O:
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include "Board.h"
#include "State.h"

int n, m, k, l;
vector<double> weights;
Board* board;
int player_id, time_limit, depth;

int test();
int test1();
int play();

int main(int argc, char** argv) {
    test1();
    // play();
    return 0;
}

int test() {
    for (int i = 0; i < 11; i++) {
        weights.push_back(i);
    }

    board = new Board(5,5,5,3);
    board->printhexagonalconfig();
    cout << endl;

    board->setMarker(make_pair(5, 6 ), 2);
    board->setMarker(make_pair(5, 7 ), 2);
    board->setMarker(make_pair(5, 8 ), 2);
    board->addRing(2, 5, 5);
    board->addRing(1, 7, 6);
    // board->setMarker(board->getCoordinates(4, 0 ), 2);
    auto adjpoints = board->showPossibleMoves(5,5);

    for(int i=0;i<adjpoints.size();i++){
        cout << adjpoints[i].first << " , " << adjpoints[i].second << " | ";
    }
    cout << endl;
    
    board->printnormalconfig();
    board->selectAndMoveRing(5,5,5,9);
    cout << "select n move done "<<endl;
    board->printnormalconfig();
    board->selectAndMoveRing(7,6,8,7);
    cout << "select n move done "<<endl;
    board->printnormalconfig();
    
    State* state = new State(board);
    cout << state->evaluate() << endl;
    cout << state->getEvaluation() << endl;
    auto tempsuccessors = state->getSuccessors(2);
    cout << "size of successors is "<<tempsuccessors.size()<<endl;
    cout<< "initial state"<<endl;
    state->stboard->printnormalconfig();
    cout<<"answers are "<<endl;
    for(int i=0;i<tempsuccessors.size();i++){
        tempsuccessors[i]->stboard->printnormalconfig();
        cout <<endl;
    }
}

int test1(){
    for (int i = 0; i < 11; i++) {
        weights.push_back(i);
    }

    board = new Board(5,5,5,3);
    State* state = new State(board);
    player_id=2;
    for(int i=0;i<10;i++){
        player_id = 3-player_id;
        state->stboard->makeInitialMoves(i/2+1);
        // state->stboard->printnormalconfig();
        // cout <<endl;
    }
    cout <<"done "<<endl;
    // state->stboard->printnormalconfig();
    state->stboard->printBeautifiedconfig();
    int temp;
    temp = state->alphaBeta(3,-DBL_MAX, +DBL_MAX, 1);
    cout<<"from"<<endl;
    state->stboard->printBeautifiedconfig();
    cout << "to"<<endl;
    state->successors[state->bestMove]->stboard->printBeautifiedconfig();
    for(int i=0;i<state->successors.size();i++){
        cout << "eval for all children is "<< state->successors[i]->getEvaluation()<<endl;
    }
}

int parseAndMove(string move) {
    pair<int,int> chosenRing = make_pair(-1,-1);
    pair<int,int> rowStart, rowEnd;
    for (unsigned i = 0; i < move.length(); i++) {
        char ch = move.at(i);
        switch (ch)
        {
            case 'P':
            {
                i += 2;
                int hex = move.at(i);
                i += 2;
                int pos = move.at(i+2);
                pair<int,int> ring = board->getCoordinates(hex, pos);
                board->addRing(player_id, ring.first, ring.second);
                break;
            }
        
            case 'S':
            {
                i += 2;
                int hex = move.at(i);
                i += 2;
                int pos = move.at(i+2);
                pair<int,int> chosenRing = board->getCoordinates(hex, pos);
                break;
            }

            case 'M':
            {
                i += 2;
                int hex = move.at(i);
                i += 2;
                int pos = move.at(i+2);
                pair<int,int> newPos = board->getCoordinates(hex,pos);
                board->selectAndMoveRing(chosenRing.first, chosenRing.second, newPos.first, newPos.second);
                break;
            }

            case 'R':
            {
                i++;
                if (move.at(i) == 'S') {
                    i += 2;
                    int hex = move.at(i);
                    i += 2;
                    int pos = move.at(i+2);
                    rowStart = board->getCoordinates(hex, pos);
                } else {
                    i += 2;
                    int hex = move.at(i);
                    i += 2;
                    int pos = move.at(i+2);
                    rowEnd = board->getCoordinates(hex, pos);
                    board->removeMarkers(rowStart.first, rowStart.second, rowEnd.first, rowEnd.second);
                }
            }

            case 'X':
            {
                i += 2;
                int hex = move.at(i);
                i += 2;
                int pos = move.at(i+2);
                pair<int,int> ring = board->getCoordinates(hex, pos);
                board->removeRing(ring.first, ring.second);
                break;
            }

            default:
                break;
        }
        i += 2;
    }
}

int play() {
    string move;
    int movenum = 1;
    // Get input from server about game specifications
    cin >> player_id >> n >> time_limit;

    if(player_id == 2) {
        // Get other player's move
        cin >> move; 
        parseAndMove(move);
    }   
    
    while(true) {
        State* currState = new State(board);
        if (movenum <= m) {
            pair<int,int> movePair = board->makeInitialMoves(movenum);
            cout << "P " << movePair.first << " " << movePair.second << endl;
        } else {
            // Check this block
            currState->alphaBeta(depth, -DBL_MAX, DBL_MAX, player_id);
            cout << currState->moves.at(currState->bestMove) << endl; // Make appropriate moves here.
            currState->makeMove();
        }
        cin >> move;
        parseAndMove(move);
    }
    return 0;
}
