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

int main ( int argc, char** argv )
{
    for (int i = 0; i < 11; i++) {
        weights.push_back(i);
    }

    board = new Board(5,5,5,3);
    // board->printhexagonalconfig();
    // cout << endl;

    // board->setMarker(board->getCoordinates(1, 0 ), 2);
    board->setMarker(board->getCoordinates(2, 0 ), 2);
    board->setMarker(board->getCoordinates(3, 0 ), 2);
    board->addRing(2, 0, 0);
    board->addRing(1, 2, 3);
    // board->setMarker(board->getCoordinates(4, 0 ), 2);
    auto adjpoints = board->showPossibleMoves(0,0);

    for(int i=0;i<adjpoints.size();i++){
        cout << adjpoints[i].first << " , " << adjpoints[i].second << " | ";
    }
    cout <<endl;
    // auto pointaftermarker = board->getPairAfterMarkers(board->getCoordinates(0,0), 90);
    // cout<< "pointaftermarker is "<< pointaftermarker.first << " "<< pointaftermarker.second << endl;
    
    board->printnormalconfig();
    board->selectAndMoveRing(0,0,4,0);
    cout << "select n move done "<<endl;
    board->printnormalconfig();
    board->selectAndMoveRing(2,3,3,5);
    cout << "select n move done "<<endl;
    board->printnormalconfig();
    
    State* state = new State(board);
    cout << state->getEvaluation() << endl;
}