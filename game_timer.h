#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>
#include "debug.h"

enum class TimeState{
  STOPPED,
  PAUSED,
  PLAYING,
};

extern unsigned long timeMs;
extern int minutes;
extern int seconds;

bool NoTime();

void Update_Time();
void Update_Timer(int mins,int secs);

void Play_Timer();
void Pause_Timer();
void Stop_Timer();

#endif