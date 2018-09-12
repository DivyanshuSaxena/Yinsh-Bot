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

int n, m, k, l;
vector<double> weights;
Board* board;

int main ( int argc, char** argv )
{
    Board *myboard = new Board(5,5,5,3);
    // auto freevecinfislope = myboard->getFreePointsAdjacentToPoint(make_pair(5,5), 90);
    myboard->printhexagonalconfig();
    cout << endl;
    // myboard->setMarker(myboard->getCoordinates(1, 0 ), 2);
    myboard->setMarker(myboard->getCoordinates(2, 0 ), 2);
    myboard->setMarker(myboard->getCoordinates(3, 0 ), 2);
    myboard->addRing(2, 0, 0);
    myboard->addRing(1, 2, 3);
    // myboard->setMarker(myboard->getCoordinates(4, 0 ), 2);
    auto adjpoints = myboard->showpossiblemoves(0,0);
    for(int i=0;i<adjpoints.size();i++){
        cout << adjpoints[i].first << " , " << adjpoints[i].second << " | ";
    }
    cout <<endl;
    // auto pointaftermarker = myboard->getPairAfterMarkers(myboard->getCoordinates(0,0), 90);
    // cout<< "pointaftermarker is "<< pointaftermarker.first << " "<< pointaftermarker.second << endl;
    
    myboard->printnormalconfig();
    myboard->selectAndMoveRing(0,0,4,0);
    cout << "select n move done "<<endl;
    myboard->printnormalconfig();
    myboard->selectAndMoveRing(2,3,3,5);
    cout << "select n move done "<<endl;
    myboard->printnormalconfig();

}