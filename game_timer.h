#ifndef __TIMER_H__
#define __TIMER_H__

// —————— headers ————————————————————————— //
#include <Arduino.h>
#include "debug.h"
// —————— headers ————————————————————————— //



// —————— enum flags —————————————————————— //
enum class TimeState{ STOPPED, PAUSED, PLAYING, };
// —————— enum flags —————————————————————— //



// —————— structures ————————————————— //
struct TimePocket {
    uint16_t days;
    uint8_t hrs;  // 23 hrs max
    uint8_t mins; // 59 mins max
    uint8_t secs; // 59 secs max
};
// —————— structures ————————————————— //



// —————— macros —————————————————————————— //
#define TO_SECONDS(_x) static_cast<int>(_x) * 1E3
#define SECS_TO_MINS 60
#define SECS_TO_HRS  3600   // 60(secs) * 60(mins)
#define SECS_TO_DAYS 86400 // 60(secs) * 60(mins) * 24(hrs)
// —————— macros —————————————————————————— {



// —————— public functions ————————————————— //
// return flags
bool NoTime();
bool TimeRunning();
bool TimeStopped();
bool TimePaused();

int GetSeconds();
int GetMinutes();

void SetStopwatch();
void StopTheWatch();
uint32_t AttainStopwatch();
TimePocket TimeConvertSeconds(uint32_t secs);
const char* TimePocketString(char* buffer, uint8_t b_size, TimePocket temp);

// update functions
void UpdateTime(bool countUp);
void UpdateTimer(int mins, int secs);

// flag toggles
void PlayTimer();
void PauseTimer();
void StopTimer();
// —————— public functions ————————————————— //

#endif