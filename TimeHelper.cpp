#include "TimeHelper.h"

TimeHelper::TimeHelper(){
    this->setClocki();
}
bool TimeHelper::outOfTime(){
    if(elapsedTimePersonal>=maxAllowedTimeSpecific){
        return true;
    }
    if(time(NULL)-clockISpecific>=maxAllowedTimeSpecific){
        return true;
    }
    return false;
}
bool TimeHelper::setClocki(){
    clocki=time(NULL);
}
bool TimeHelper::setMaxAllowedTime(int argtime){
    maxAllowedTime=argtime;
}
bool TimeHelper::setClockISpecific(){
    clockISpecific = time(NULL);
}
bool TimeHelper::setMaxAllowedTimeSpecific(float argtime){
    maxAllowedTimeSpecific=argtime;
}
bool TimeHelper::updateElapsedTimePersonal(){
    elapsedTimePersonal += time(NULL)-clockISpecific;
}