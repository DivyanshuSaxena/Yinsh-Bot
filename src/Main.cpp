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
#include <stdlib.h>

int n, m, k, l;
vector<double> weights;

ofstream outfile;
ofstream outfileShaved;

bool DEBUG_EVAL;
bool NON_FLIP;
bool WRITE_FILE;
double DBLMAX = 99999999;
double THRESHOLD = -10000000;

Board* board;
int player_id, time_limit, max_depth;
TimeHelper * timeHelper;
string player1_weight = "./Yinsh-Bot/train/player1.dat";
string player2_weight = "./Yinsh-Bot/train/player2.dat";
string best_weight = "./Yinsh-Bot/train/best.log";

int test();
int test1();
int test2();
int boardhelper();
int play();

int main(int argc, char** argv) {
    // std::srand ( unsigned ( std::time(0) ) );
    timeHelper = new TimeHelper();

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
    
    State* state = new State(board, 1);
    outfile << state->getEvaluation() << endl;
    outfile << state->getEvaluation() << endl;
    auto tempsuccessors = state->getSuccessors(2);
    outfile << "size of successors is "<<tempsuccessors.size()<<endl;
    outfile<< "initial state"<<endl;
    state->stboard->printnormalconfig();
    outfile<<"answers are "<<endl;
    for(int i=0;i<tempsuccessors.size();i++){
        tempsuccessors[i].first->stboard->printnormalconfig();
        outfile <<endl;
    }
}

void setWeights() {
    // Initialize weights
    double w[] = {0,5,15,12,25,200,2,600};
    // double w[] = {0,4,12,15,28,250,2,800};
    weights.push_back(0);
    for (int i = 1; i <= 7; i++) {
        if (player_id == 1) {
            weights.push_back(w[i]);
            weights.push_back(-w[i]);
        } else {
            weights.push_back(-w[i]);
            weights.push_back(w[i]);
        }
    }
}

void generate_weights() {
    int rows_k2[] = {2,10};
    int rows_k2_nonflip[] = {10,20};
    int rows_k1[] = {8,16};
    int rows_k1_nonflip[] = {18,32};
    int rows_k[] = {150,250};
    int blocked_dof[] = {1,8};
    int ring_removed[] = {400,800};

    int weight_limit[][2] = {
        {rows_k2[0], rows_k2[1]}, 
        {rows_k2_nonflip[0], rows_k2_nonflip[1]}, 
        {rows_k1[0], rows_k1[1]}, 
        {rows_k1_nonflip[0], rows_k1_nonflip[1]}, 
        {rows_k[0], rows_k[1]}, 
        {blocked_dof[0], blocked_dof[1]}, 
        {ring_removed[0], ring_removed[1]}
    };

    ofstream weights_file;
    if (player_id == 1) {
        weights_file.open(player1_weight);
    } else {
        weights_file.open(player2_weight);
    }
    weights_file << "0\n";

    weights.push_back(0);
    for (int i = 0; i < 7; i++) {
        double increment = (weight_limit[i][1] - weight_limit[i][0])/10;
        if (increment < 1)
            increment = 1;
        int above = rand() % 11;
        int weight = weight_limit[i][0] + above * increment;
        weights_file << weight << " ";
        if (player_id == 1) {
            weights.push_back(weight);
            weights.push_back(-weight);
        } else {
            weights.push_back(-weight);
            weights.push_back(weight);
        }
    }
    weights_file.close();
}

void update_weights() {
    ifstream other_weight;
    if (player_id == 1) {
        other_weight.open(player2_weight);
    } else {
        other_weight.open(player1_weight);
    }

    vector<double> winner_weights;
    winner_weights.push_back(0);
    for (int i = 1; i <= 7; i++) {
        int weight;
        other_weight >> weight;
        if (player_id == 1) {
            winner_weights.push_back(weight);
            winner_weights.push_back(-weight);
        } else {
            winner_weights.push_back(-weight);
            winner_weights.push_back(weight);
        }
    }

    // double alpha = 0.5;
    // for (int i = 1; i <= 7; i++) {
    //     weights[i] += alpha * (winner_weights[i] - weights[i]);
    // }
}

int test2(){
    board = new Board(5,5,5,3);
    board->createBoardFromFile("debug/board.txt");
    board->printnormalconfigShaved();
    outfileShaved << "lets do search" << endl;
    player_id = 2;
    setWeights();
    State * state = new State(board, player_id);
    int temp;
    timeHelper->setMaxAllowedTime(150);
    timeHelper->setClockISpecific();
    timeHelper->setMaxAllowedTimeSpecific(3);
    outfileShaved<<"state is "<<endl;
    state->stboard->printnormalconfigShaved();
    state->stboard->printBeautifiedconfigShaved();
    DEBUG_EVAL = true;
    outfile << state->getEvaluation() << endl;
    // DEBUG_EVAL = false;
    // NON_FLIP = true;
    // WRITE_FILE = true;
    // max_depth = 4;
    // temp = state->iterativeDeepening(max_depth, player_id);
    // outfile << "Best Move at: " << state->bestMove << endl; // Debug
    // cout << state->successors.at(state->bestMove).second << endl; // Make appropriate moves here.
    // outfileShaved<< "I, player"<< state->playerToMove <<" did "<< state->successors.at(state->bestMove).second << endl;
    // outfileShaved<<"fin state is "<<endl;
    // state->successors[state->bestMove].first->stboard->printnormalconfigShaved();
    // state->successors[state->bestMove].first->stboard->printBeautifiedconfigShaved();
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
    outfile << "The initial inputs are "<<endl;
    outfile << basics << endl;
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

                case 2:
                    time_limit = stoi(word);
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
    k = stoi(word);
    
    // Initialize hyper parameters
    NON_FLIP = true;
    DEBUG_EVAL = false;
    WRITE_FILE = false;
    timeHelper->setMaxAllowedTime(time_limit);
    if(n==5){
        max_depth = 5;
    }else{
        max_depth = 4;
    }
    // max_depth = 4;
    
    // Initialize streams
    if (player_id == 1) {
        outfile.open("console1.log");
        outfileShaved.open("consoleshaved1.log");
    } else {
        outfile.open("console2.log");
        outfileShaved.open("consoleshaved2.log");
    }

    string move;
    int movenum = 1;
    board = new Board(n, n, k, 3); //n=m in cases
    // board = new Board(n,5,5,3);
    
    // Get Weights
    fstream weights_file;
    if (player_id == 1) {
        outfileShaved << "Opening 1.txt" << endl;
        // weights_file.open("Yinsh-Bot/train/player1.dat", std::ios_base::in);
        weights_file.open(player1_weight, std::ios_base::in);
    } else {
        outfileShaved << "Opening 2.txt" << endl;
        // weights_file.open("Yinsh-Bot/train/player2.dat", std::ios_base::in);
        weights_file.open(player2_weight, std::ios_base::in);
    }
    int is_winner, num_times;
    weights_file >> is_winner >> num_times;
    outfileShaved << is_winner << " " << num_times << endl;
    
    // Read weights from existing file.
    weights.push_back(0);
    for (int i = 1; i <= 7; i++) {
        double weight;
        weights_file >> weight;
        outfileShaved << "Read from file: " << weight << endl;
        if (player_id == 1) {
            weights.push_back(weight);
            weights.push_back(-weight);
        } else {
            weights.push_back(-weight);
            weights.push_back(weight);
        }
    }
    if (is_winner == 1) {
        if (num_times > 10) {
            // Back up this weight in a file and generate a new weight
            ofstream best_file;
            best_file.open(best_weight, std::ios_base::app);
            for (double w : weights)
                best_file << w << " ";
            generate_weights();
        }
    } else {
        update_weights();
    }
    // setWeights();

    // Print weights
    for (double w : weights) 
        outfileShaved << w << " " << endl;

    if(player_id == 2) {
        // Get other player's move
        getline(cin, move); 
        parseAndMove(move);
    }
    while(true) {
        timeHelper->setClockISpecific();
        timeHelper->setMaxAllowedTimeSpecific(3);
        State* currState = new State(board, player_id);
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
            // currState->alphaBeta(depth, -DBLMAX, DBLMAX, player_id);
            outfile << "Starting ID" << endl;
            currState->iterativeDeepening(max_depth, player_id);
            outfile << "Best Move at: " << currState->bestMove << endl; // Debug
            //nothing does that
            cout << currState->successors.at(currState->bestMove).second << endl; // Make appropriate moves here.
            outfileShaved<< "I, player "<<currState->playerToMove <<" did "<< currState->successors.at(currState->bestMove).second  << " which means "<< endl;
            currState->successors[currState->bestMove].first->stboard->printnormalconfigShaved();
            currState->successors[currState->bestMove].first->stboard->printBeautifiedconfigShaved();
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
