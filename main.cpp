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
int player_id;
int time_limit;

int main(int argc, char** argv) {
    test();
    // string move;
    // // Get input from server about game specifications
    // cin >> player_id >> n >> time_limit;

    // if(player_id == 2) {
    //     // Get other player's move
    //     cin>>move; 
        
    //     while(true) {
    //         cout<<"P 1 0"<<endl;
    //         cin>>move;
    //     }
    // }   
    // else if(player_id == 1) {
    //     while(true) {
    //         cout<<"P 0 0"<<endl;
    //         cin>>move; 
    //     }
    // }
    // return 0;
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
}