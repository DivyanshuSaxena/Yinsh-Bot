/* 
 * File:   Time.h
 * Author: Akshat Khare Divyanshu Saxena
 * 
 */
#ifndef Time_H
#define	Time_H
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
class Time{
    private:
        static int clocki;
        static int maxAllowedTime;
        static int clockISpecific;
        static float maxAllowedTimeSpecific;
        static int elapsedTimePersonal;
    public:
        static bool outOfTime();
        static bool setClocki();
        static bool setMaxAllowedTime(int);
        static bool setClockISpecific();
        static bool setMaxAllowedTimeSpecific(float);
        static bool updateElapsedTimePersonal();
}
#endif /* Time_H */