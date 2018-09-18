#include "TimeHelper.h"

TimeHelper::TimeHelper(){
    this->setClocki();
    this->averageNumMoves=60;
}
bool TimeHelper::outOfTime(){
    // if(elapsedTimePersonal>=maxAllowedTimeSpecific){
    //     cerr<<"time out"<<endl;
    //     return true;
    // }
    // if(time(NULL)-clockISpecific>=maxAllowedTimeSpecific){
    //     cerr << "time out"<<endl;
    //     return true;
    // }
    return false;
}
bool TimeHelper::setClocki(){
    clocki=time(NULL);
    this->halfMovesCount = this->halfMovesCount+1;
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
bool TimeHelper::setMaxAllowedTimeSpecificAutomatic(){
    float estimateHalfmovesRemaining = this->averageNumMoves/2.0 - this->halfMovesCount;
    if(estimateHalfmovesRemaining<3){
        estimateHalfmovesRemaining=5;
    }
    float timeToBeDevoted = (maxAllowedTime-elapsedTimePersonal)/estimateHalfmovesRemaining;
    this->setMaxAllowedTimeSpecific(timeToBeDevoted);
}