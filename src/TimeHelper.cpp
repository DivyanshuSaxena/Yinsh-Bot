#include "TimeHelper.h"

TimeHelper::TimeHelper(){
    this->setClocki();
}
bool TimeHelper::outOfTime(){
    // cerr << "specific time elapsed is "<< difftime(time(NULL),startprocess) << endl;
    if(elapsedTimePersonal>=maxAllowedTime){
        cerr << "total out of time"<<endl;
        return true;
    }
    time(&endprocess);
    if(difftime(endprocess,startprocess)>=maxAllowedTimeSpecific){
        cerr << "specific process out of time "<< difftime(endprocess,startprocess) <<endl;
        return true;
    }
    return false;
}
bool TimeHelper::setClocki(){
    // clocki=time(NULL);
    time(&start);
}
bool TimeHelper::setMaxAllowedTime(int argtime){
    maxAllowedTime=argtime;
}
bool TimeHelper::setClockISpecific(){
    // clockISpecific = time(NULL);
    time(&starttemp);
}
bool TimeHelper::setMaxAllowedTimeSpecific(float argtime){
    maxAllowedTimeSpecific=argtime;
    time(&startprocess);
}
bool TimeHelper::updateElapsedTimePersonal(){
    // elapsedTimePersonal += time(NULL)-clockISpecific;
    time(&endtemp);
    elapsedTimePersonal += difftime(endtemp, starttemp);
}