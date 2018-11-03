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
#include <cfloat>
using namespace std;

class TimeHelper{
    // private:
    public:
        int clocki;
        int maxAllowedTime;
        int clockISpecific;
        float maxAllowedTimeSpecific;
        int elapsedTimePersonal;
    // public:
        TimeHelper();
        bool outOfTime();
        bool setClocki();
        bool setMaxAllowedTime(int);
        bool setClockISpecific();
        bool setMaxAllowedTimeSpecific(float);
        bool updateElapsedTimePersonal();
};
#endif /* Time_Helper */