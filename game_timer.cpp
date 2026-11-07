#include "game_timer.h"

<<<<<<< HEAD
//————/ GLOBAL VARIABLES /————————————//
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
unsigned long timeMs=0;
int minutes=0;
int seconds=0;
TimeState timeStatus=TimeState::STOPPED;
<<<<<<< HEAD
//————/ GLOBAL VARIABLES /————————————//

//————/ RETURN FUNCTIONS /————————————//
int GetSeconds() { return seconds; }
int GetMinutes() { return minutes; }
=======

>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
bool NoTime() { return (minutes<=0)&&(seconds<=0); }
bool TimeRunning() { return timeStatus==TimeState::PLAYING; }
bool TimeStopped() { return timeStatus==TimeState::STOPPED; }
bool TimePaused() { return timeStatus==TimeState::PAUSED; }
<<<<<<< HEAD
//————/ RETURN FUNCTIONS /————————————//

//————/ FUNCTIONS /————————————//
void PlayTimer() {
=======

void Play_Timer() {
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  if (TimeRunning()) return;
  timeStatus=TimeState::PLAYING;
}

<<<<<<< HEAD
void PauseTimer() {
=======
void Pause_Timer() {
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  if (TimePaused()) return;
  timeStatus=TimeState::PAUSED;
}

<<<<<<< HEAD
void StopTimer() {
  if (TimeStopped()) return;
  timeStatus=TimeState::STOPPED;
  minutes=0; seconds=0;
}

void UpdateTime(bool countUp) {
=======
void Stop_Timer() {
  if (TimeStopped()) return;
  timeStatus=TimeState::STOPPED;
  minutes=0;
  seconds=0;
}

void Update_Time() {
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  if (TimeRunning()) {
    const unsigned long ms=millis();
    const unsigned long diffMs=(ms-timeMs);
    if (diffMs>=1E3) {
      timeMs=ms;
<<<<<<< HEAD
      if (!countUp) {
        bool outOfSeconds=(seconds<=0);
        bool hasMinutes=(minutes>0);
        bool minuteReset=(hasMinutes && outOfSeconds);
        minutes=(minuteReset)?minutes-1:minutes;
        seconds=(minuteReset)?59:seconds-1;
        // DEBUG_Time();
        if (NoTime()) { StopTimer(); }
      } else {
        
=======
      bool outOfSeconds=(seconds<=0);
      bool hasMinutes=(minutes>0);
      bool minuteReset=(hasMinutes && outOfSeconds);
      minutes=(minuteReset)?minutes-1:minutes;
      seconds=(minuteReset)?59:seconds-1;
      DEBUG_Time();
      if (NoTime()) {
        Stop_Timer();
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
      }
    }
  }
}

<<<<<<< HEAD
void UpdateTimer(int mins,int secs) {
=======


void Update_Timer(int mins,int secs) {
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  int mTemp=minutes+mins;
  int sTemp=seconds+secs;
  while (sTemp>=60) {
    mTemp++;
    sTemp=sTemp%60;
  }
  minutes=mTemp;
  seconds=sTemp;
<<<<<<< HEAD
}
//————/ FUNCTIONS /————————————//
=======
}
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
