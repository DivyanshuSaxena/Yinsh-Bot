/* 
 * File:   State.h
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
#ifndef State_H
#define	State_H

#include <iostream>
#include <fstream>
#include <vector>
#include "Board.h"
using namespace std;

class State {
    public:
        // Heuristics
        double getLinearMarkers();
        double getEvaluation();
};
#endif /* State_H */