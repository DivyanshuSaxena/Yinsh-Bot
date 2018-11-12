/* 
 * File:   TimeHelper.h
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
#ifndef Time_Helper
#define	Time_Helper
#include <iostream>
#include <fstream>
#include <ctime>
#include <time.h>
#include <cfloat>
// #include "Board.h"
// #include "State.h"
using namespace std;

class TimeHelper{
    // private:
    public:
        bool TIME_DEBUG =false;
        int clocki;
        int maxAllowedTime;
        double clockISpecific;
        double maxAllowedTimeSpecific;
        double elapsedTimePersonal;
        clock_t start, starttemp, endtemp, startprocess, endprocess;
    // public:
        TimeHelper(bool debug);
        bool outOfTime();
        bool setClocki();
        bool setMaxAllowedTime(int);
        bool setClockISpecific();
        bool setMaxAllowedTimeSpecific(double, int movenum, int n, int k);
        bool updateElapsedTimePersonal();
};
#endif /* Time_Helper */