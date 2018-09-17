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
#include "TimeHelper.h"
#include "Board.h"
#include "State.h"
#include <ctime>

int n, m, k, l;
ofstream outfile;
ofstream outfileShaved;
vector<double> weights;

Board* board;
int player_id, time_limit, max_depth;

int test();
int test1();
int test2();
int boardhelper();
int play();
TimeHelper * timeHelper;
int main(int argc, char** argv) {
    std::srand ( unsigned ( std::time(0) ) );
    timeHelper = new TimeHelper();

    // Initialize streams
    outfile.open("console.log");
    outfileShaved.open("consoleshaved.log");

    // test1();
    play();
    // test2();
    // boardhelper();
    return 0;
}

int test() {
    board = new Board(5,5,5,3);
    board->printhexagonalconfig();
    outfile << endl;

    board->setMarker(make_pair(5, 6 ), 2);
    board->setMarker(make_pair(5, 7 ), 2);
    board->setMarker(make_pair(5, 8 ), 2);
    board->addRing(2, 5, 5);
    board->addRing(1, 7, 6);
    // board->setMarker(board->getCoordinates(4, 0 ), 2);
    auto adjpoints = board->showPossibleMoves(5,5);

    for(int i=0;i<adjpoints.size();i++){
        outfile << adjpoints[i].first << " , " << adjpoints[i].second << " | ";
    }
    outfile << endl;
    
    board->printnormalconfig();
    board->selectAndMoveRing(5,5,5,9);
    outfile << "select n move done "<<endl;
    board->printnormalconfig();
    board->selectAndMoveRing(7,6,8,7);
    outfile << "select n move done "<<endl;
    board->printnormalconfig();
    
    State* state = new State(board);
    outfile << state->evaluate() << endl;
    outfile << state->getEvaluation() << endl;
    auto tempsuccessors = state->getSuccessors(2);
    outfile << "size of successors is "<<tempsuccessors.size()<<endl;
    outfile<< "initial state"<<endl;
    state->stboard->printnormalconfig();
    outfile<<"answers are "<<endl;
    for(int i=0;i<tempsuccessors.size();i++){
        tempsuccessors[i]->stboard->printnormalconfig();
        outfile <<endl;
    }
}

int test1(){
    board = new Board(5,5,5,3);
    State* state = new State(board);
    player_id=2;
    for(int i=0;i<10;i++){
        player_id = 3-player_id;
        state->stboard->makeInitialMoves(i/2+1);
        // state->stboard->printnormalconfig();
        // outfile <<endl;
    }
    outfile <<"done "<<endl;
    // state->stboard->printnormalconfig();
    state->stboard->printnormalconfig();
    int temp;
    temp = state->alphaBeta(3,-DBL_MAX, +DBL_MAX, 1);
    outfile<<"from"<<endl;
    state->stboard->printnormalconfig();
    outfile << "to"<<endl;
    state->successors[state->bestMove]->stboard->printnormalconfig();
    for(int i=0;i<state->successors.size();i++){
        outfile << "eval for all children is "<< state->successors[i]->getEvaluation()<<endl;
    }
    // int templayer =2;
    // state->evaluate();
    // while(true){
    //     int temp;
    //     temp = state->alphaBeta(2,-DBL_MAX, +DBL_MAX, 3-templayer);
    //     cout<<"from"<<endl;
    //     state->stboard->printnormalconfig();
    //     cout << "to"<<endl;
    //     state->successors[state->bestMove]->stboard->printnormalconfig();
    //     state = state->successors[state->bestMove];
    //     // state->evaluate();
    //     // state = null;
    // }
}
int test2(){
    weights.push_back(0);
    for (int i = 1; i <= 6; i++) {
        if (player_id == 1) {
            weights.push_back(i);
            weights.push_back(-i);
        } else {
            weights.push_back(-i);
            weights.push_back(i);
        }
    }
    board = new Board(5,5,5,3);
    board->createBoardFromFile("board.txt");
    board->printnormalconfigShaved();
    board->printBeautifiedconfigShaved();
    outfileShaved<<"lets do search"<<endl;
    State * state = new State(board);
    int temp;
    timeHelper->setMaxAllowedTime(150);
    timeHelper->setClockISpecific();
    timeHelper->setMaxAllowedTimeSpecific(3);
    outfileShaved<<"state is "<<endl;
    state->stboard->printnormalconfigShaved();
    state->stboard->printBeautifiedconfigShaved();
    temp = state->iterativeDeepening(2,2);
    outfile << "Best Move at: " << state->bestMove << endl; // Debug
    cout << state->moves.at(state->bestMove) << endl; // Make appropriate moves here.
    outfileShaved<< "I did "<< state->moves.at(state->bestMove) << endl;
    outfileShaved<<"fin state is "<<endl;
    state->successors[state->bestMove]->stboard->printnormalconfigShaved();
    state->successors[state->bestMove]->stboard->printBeautifiedconfigShaved();
}

int boardhelper(){
    board=new Board(5,5,5,3);
    board->printhexagonalconfig();
}

void parseAndMove(string move) {
    vector<string> components;
    string word = "";
    for (int i = 0; i < move.length(); i++) {
        char ch = move.at(i);
        if (ch == ' ') {
            components.push_back(word);
            // cout << word << endl;
            word = "";
        } else {
            word = word + ch;
        }
    }
    components.push_back(word);

    pair<int,int> chosenRing = make_pair(-1,-1);
    pair<int,int> rowStart, rowEnd;
    for (int i = 0; i < components.size(); ) {
        string s = components.at(i++);
        if (s == "P") {
            int hex = stoi(components.at(i++));
            int pos = stoi(components.at(i++));
            pair<int,int> ring = board->getCoordinates(hex, pos);
            board->addRing(3-player_id, ring.first, ring.second);
        } else if (s == "S") {
            int hex = stoi(components.at(i++));
            int pos = stoi(components.at(i++));
            chosenRing = board->getCoordinates(hex, pos);
        } else if (s == "M") {
            int hex = stoi(components.at(i++));
            int pos = stoi(components.at(i++));
            pair<int,int> newPos = board->getCoordinates(hex,pos);
            board->selectAndMoveRing(chosenRing.first, chosenRing.second, newPos.first, newPos.second);
        } else if (s == "RS") {
            int hex = stoi(components.at(i++));
            int pos = stoi(components.at(i++));
            rowStart = board->getCoordinates(hex, pos);            
        } else if (s == "RE") {
            int hex = stoi(components.at(i++));
            int pos = stoi(components.at(i++));
            rowEnd = board->getCoordinates(hex, pos);
            board->removeMarkers(rowStart.first, rowStart.second, rowEnd.first, rowEnd.second);
        } else if (s == "X") {
            int hex = stoi(components.at(i++));
            int pos = stoi(components.at(i++));
            pair<int,int> ring = board->getCoordinates(hex, pos);
            board->removeRing(ring.first, ring.second);
        }
    }
    // cout << "Made move" << endl;
}

int play() {
    // Get input from server about game specifications
    string basics;
    getline(cin, basics);
    string word = "";
    int input = 0;
    for (int i = 0; i < basics.length(); i++) {
        char ch = basics.at(i);
        if (ch == ' ') {
            switch (input)
            {
                case 0:
                    player_id = stoi(word);
                    input++;
                    word = "";
                    break;

                case 1:
                    n = stoi(word);
                    input++;
                    word = "";
                    break;

                default:
                    break;
            }
        } else {
            word = word + to_string(ch-48);
        }
    }
    time_limit = stoi(word);
    
    timeHelper->setMaxAllowedTime(time_limit);
    max_depth = 2;

    string move;
    int movenum = 1;
    board = new Board(n,5,5,3);

    // Initialize weights
    weights.push_back(0);
    for (int i = 1; i <= 6; i++) {
        if (player_id == 1) {
            weights.push_back(i);
            weights.push_back(-i);
        } else {
            weights.push_back(-i);
            weights.push_back(i);
        }
    }

    if(player_id == 2) {
        // Get other player's move
        getline(cin, move); 
        parseAndMove(move);
    }
    while(true) {
        timeHelper->setClockISpecific();
        timeHelper->setMaxAllowedTimeSpecific(3);
        State* currState = new State(board);
        outfileShaved<<"lets see this one"<<endl;
        board->printnormalconfigShaved();
        board->printBeautifiedconfigShaved();
        outfileShaved<< "starting iteration on above"<<endl;
        // outfile << movenum << endl;           
        if (movenum <= m) {
            pair<int,int> movePair = board->makeInitialMoves(movenum);
            cout << "P " << movePair.first << " " << movePair.second << endl;
            outfileShaved<< "I did "<< "P " << movePair.first << " " << movePair.second << endl;
        } else {
            // Check this block
            // currState->alphaBeta(depth, -DBL_MAX, DBL_MAX, player_id);
            outfile << "Starting ID" << endl;
            currState->iterativeDeepening(max_depth, player_id);
            outfile << "Best Move at: " << currState->bestMove << endl; // Debug
            cout << currState->moves.at(currState->bestMove) << endl; // Make appropriate moves here.
            outfileShaved<< "I did "<< currState->moves.at(currState->bestMove)  << "which means "<< endl;
            currState->successors[currState->bestMove]->stboard->printnormalconfigShaved();
            currState->successors[currState->bestMove]->stboard->printBeautifiedconfigShaved();
            currState->makeMove();
        }
        outfile << "Moved self move" << endl;
        getline(cin, move);
        outfileShaved << "He did "<< move << endl;
        parseAndMove(move);
        timeHelper->updateElapsedTimePersonal();
        outfile << "Opponent move done" << endl;
        movenum++;
    }
    return 0;
}
