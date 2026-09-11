#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>
#include "debug.h"

// ——————————————————— enum flags —————————
enum class TimeState{ STOPPED,PAUSED,PLAYING, };
// ————————————————————————————————————————



// ——————————————————— global vars ————————
extern unsigned long timeMs;
extern int minutes;
extern int seconds;
// ————————————————————————————————————————



// ——————————————————— functions ——————————
// return flags
bool NoTime();
bool TimeRunning();
bool TimeStopped();
bool TimePaused();

// update functions
void Update_Time();
void Update_Timer(int mins,int secs);

// flag toggles
void Play_Timer();
void Pause_Timer();
void Stop_Timer();
// ————————————————————————————————————————

#endif