#ifndef __TIMER_H__
#define __TIMER_H__

// ——————————————————— headers ————————————
#include <Arduino.h>
#include "debug.h"
// ————————————————————————————————————————



// ——————————————————— enum flags —————————
enum class TimeState{ STOPPED,PAUSED,PLAYING, };
// ————————————————————————————————————————



// ——————————————————— global vars ————————
extern unsigned long timeMs;
// ————————————————————————————————————————



// ——————————————————— functions ——————————
// return flags
bool NoTime();
bool TimeRunning();
bool TimeStopped();
bool TimePaused();

int GetSeconds();
int GetMinutes();

// update functions
void UpdateTime(bool countUp);
void UpdateTimer(int mins,int secs);

// flag toggles
void PlayTimer();
void PauseTimer();
void StopTimer();
// ————————————————————————————————————————

#endif