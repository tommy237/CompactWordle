#include "game_timer.h"

//————/ GLOBAL VARIABLES /————————————//
unsigned long timeMs=0;
int minutes=0;
int seconds=0;
TimeState timeStatus=TimeState::STOPPED;
//————/ GLOBAL VARIABLES /————————————//

//————/ RETURN FUNCTIONS /————————————//
int GetSeconds() { return seconds; }
int GetMinutes() { return minutes; }
bool NoTime() { return (minutes<=0)&&(seconds<=0); }
bool TimeRunning() { return timeStatus==TimeState::PLAYING; }
bool TimeStopped() { return timeStatus==TimeState::STOPPED; }
bool TimePaused() { return timeStatus==TimeState::PAUSED; }
//————/ RETURN FUNCTIONS /————————————//

//————/ FUNCTIONS /————————————//
void PlayTimer() {
  if (TimeRunning()) return;
  timeStatus=TimeState::PLAYING;
}

void PauseTimer() {
  if (TimePaused()) return;
  timeStatus=TimeState::PAUSED;
}

void StopTimer() {
  if (TimeStopped()) return;
  timeStatus=TimeState::STOPPED;
  minutes=0; seconds=0;
}

void UpdateTime(bool countUp) {
  if (TimeRunning()) {
    const unsigned long ms=millis();
    const unsigned long diffMs=(ms-timeMs);
    if (diffMs>=1E3) {
      timeMs=ms;
      if (!countUp) {
        bool outOfSeconds=(seconds<=0);
        bool hasMinutes=(minutes>0);
        bool minuteReset=(hasMinutes && outOfSeconds);
        minutes=(minuteReset)?minutes-1:minutes;
        seconds=(minuteReset)?59:seconds-1;
        // DEBUG_Time();
        if (NoTime()) { StopTimer(); }
      } else {
        
      }
    }
  }
}

void UpdateTimer(int mins,int secs) {
  int mTemp=minutes+mins;
  int sTemp=seconds+secs;
  while (sTemp>=60) {
    mTemp++;
    sTemp=sTemp%60;
  }
  minutes=mTemp;
  seconds=sTemp;
}
//————/ FUNCTIONS /————————————//