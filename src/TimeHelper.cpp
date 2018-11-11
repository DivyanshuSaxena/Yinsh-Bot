#include "TimeHelper.h"

TimeHelper::TimeHelper(bool debug){
    TIME_DEBUG =debug;
    this->setClocki();
}
bool TimeHelper::outOfTime(){
    // if(TIME_DEBUG) cerr << "specific time elapsed is "<< difftime(time(NULL),startprocess) << endl;
    if(elapsedTimePersonal>=maxAllowedTime){
        if(TIME_DEBUG) cerr << "total out of time"<<endl;
        return true;
    }
    // time(&endprocess);
    endprocess = clock();
    // if(difftime(endprocess,startprocess)>=maxAllowedTimeSpecific){
    if((endprocess-startprocess)/(CLOCKS_PER_SEC)>=maxAllowedTimeSpecific){

        if(TIME_DEBUG) cerr << "specific process out of time "<< (endprocess-startprocess)/(CLOCKS_PER_SEC) <<endl;
        return true;
    }
    return false;
}
bool TimeHelper::setClocki(){
    // clocki=time(NULL);
    // time(&start);
    start =clock();
}
bool TimeHelper::setMaxAllowedTime(int argtime){
    maxAllowedTime=argtime;
}
bool TimeHelper::setClockISpecific(){
    // clockISpecific = time(NULL);
    // time(&starttemp);
    starttemp = clock();
}
bool TimeHelper::setMaxAllowedTimeSpecific(double argtime, int movenum, int n, int k){
    if(n==5){
        int maxmoves = 30;
        if(movenum>=maxmoves){
            maxAllowedTimeSpecific = argtime;
        }else{
            maxAllowedTimeSpecific = max(argtime,(maxAllowedTime-elapsedTimePersonal)/(maxmoves-movenum));
        }
    }else if(n==6 && k==6){
        int maxmoves = 40;
        if(movenum>=maxmoves){
            maxAllowedTimeSpecific = argtime;
        }else{
            maxAllowedTimeSpecific = max(argtime,(maxAllowedTime-elapsedTimePersonal)/(maxmoves-movenum));
        }
    }else if(n==6 && k==5){
        int maxmoves = 30;
        if(movenum>=maxmoves){
            maxAllowedTimeSpecific = argtime;
        }else{
            maxAllowedTimeSpecific = max(argtime,(maxAllowedTime-elapsedTimePersonal)/(maxmoves-movenum));
        }
    }
    if(TIME_DEBUG) cerr << "maxtime alloted is "<< maxAllowedTimeSpecific<<endl;
    // time(&startprocess);
    startprocess = clock();
}
bool TimeHelper::updateElapsedTimePersonal(){
    // elapsedTimePersonal += time(NULL)-clockISpecific;
    // time(&endtemp);
    // elapsedTimePersonal += difftime(endtemp, starttemp);
    endtemp = clock();
    elapsedTimePersonal += (double (endtemp-starttemp))/(CLOCKS_PER_SEC);
}