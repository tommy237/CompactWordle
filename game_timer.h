#ifndef __TIMER_H__
#define __TIMER_H__

<<<<<<< HEAD
// ——————————————————— headers ————————————
#include <Arduino.h>
#include "debug.h"
// ————————————————————————————————————————


=======
#include <Arduino.h>
#include "debug.h"
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c

// ——————————————————— enum flags —————————
enum class TimeState{ STOPPED,PAUSED,PLAYING, };
// ————————————————————————————————————————



// ——————————————————— global vars ————————
extern unsigned long timeMs;
<<<<<<< HEAD
=======
extern int minutes;
extern int seconds;
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ————————————————————————————————————————



// ——————————————————— functions ——————————
// return flags
bool NoTime();
bool TimeRunning();
bool TimeStopped();
bool TimePaused();

<<<<<<< HEAD
int GetSeconds();
int GetMinutes();

// update functions
void UpdateTime(bool countUp);
void UpdateTimer(int mins,int secs);

// flag toggles
void PlayTimer();
void PauseTimer();
void StopTimer();
=======
// update functions
void Update_Time();
void Update_Timer(int mins,int secs);

// flag toggles
void Play_Timer();
void Pause_Timer();
void Stop_Timer();
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ————————————————————————————————————————

#endif