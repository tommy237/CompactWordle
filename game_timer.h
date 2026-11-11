#ifndef __TIMER_H__
#define __TIMER_H__

// —————— headers ————————————————————————— //
#include <Arduino.h>
#include "debug.h"
// —————— headers ————————————————————————— //



// —————— enum flags —————————————————————— //
enum class TimeState{ STOPPED, PAUSED, PLAYING, };
// —————— enum flags —————————————————————— //



// —————— macros —————————————————————————— //
#define TO_SECONDS(_x) static_cast<int>(_x)*1E3
// —————— macros —————————————————————————— //



// —————— public functions ————————————————— //
// return flags
bool NoTime();
bool TimeRunning();
bool TimeStopped();
bool TimePaused();

int GetSeconds();
int GetMinutes();

// update functions
void UpdateTime(bool countUp);
void UpdateTimer(int mins, int secs);

// flag toggles
void PlayTimer();
void PauseTimer();
void StopTimer();
// —————— public functions ————————————————— //

#endif