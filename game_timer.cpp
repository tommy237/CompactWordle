#include "game_timer.h"

unsigned long timeMs=0;
int minutes=0;
int seconds=0;
TimeState timeStatus=TimeState::STOPPED;

bool NoTime() { return (minutes<=0)&&(seconds<=0); }

void Update_Time() {
  if (timeStatus==TimeState::PLAYING) {
    const unsigned long ms=millis();
    const unsigned long diffMs=(ms-timeMs);
    if (diffMs>=1E3) {
      timeMs=ms;
      bool outOfSeconds=(seconds<=0);
      bool hasMinutes=(minutes>0);
      bool minuteReset=(hasMinutes && outOfSeconds);
      minutes=(minuteReset)?minutes-1:minutes;
      seconds=(minuteReset)?59:seconds-1;
      DEBUG_Time();
      if (!hasMinutes && outOfSeconds) {
        timeStatus==TimeState::STOPPED;
      }
    }
  }
}

void Play_Timer() {
  timeStatus=TimeState::PLAYING;
}

void Pause_Timer() {
  timeStatus=TimeState::PAUSED;
}

void Stop_Timer() {
  timeStatus=TimeState::STOPPED;
  minutes=0;
  seconds=0;
}

void Update_Timer(int mins,int secs) {
  int mTemp=minutes+mins;
  int sTemp=seconds+secs;
  while (sTemp>=60) {
    mTemp++;
    sTemp=sTemp%60;
  }
  minutes=mTemp;
  seconds=sTemp;
}