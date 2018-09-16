#include "Time.h"

bool Time::outOfTime(){
    if(elapsedTimePersonal>=maxAllowedTimeSpecific){
        return true;
    }
    if(time(NULL)-clockISpecific>=maxAllowedTimeSpecific){
        return true;
    }
    return false;
}
bool Time::setClocki(){
    clocki=time(NULL);
}
bool Time::setMaxAllowedTime(int argtime){
    maxAllowedTime=argtime;
}
bool Time::setClockISpecific(){
    clockISpecific = time(NULL);
}
bool Time::setMaxAllowedTimeSpecific(float argtime){
    maxAllowedTimeSpecific=argtime;
}
bool Time::updateElapsedTimePersonal(){
    elapsedTimePersonal += time(NULL)-clockISpecific;
}