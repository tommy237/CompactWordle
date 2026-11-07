#ifndef __DEBUG_H__
#define __DEBUG_H__

<<<<<<< HEAD
// ——————————————————— headers ————————————
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
#include <Arduino.h>
#include "buttons.h"
#include "lcd.h"
#include "wordle_prog.h"
#include "game_timer.h"
<<<<<<< HEAD
// ————————————————————————————————————————


// ——————————————————— enum flags —————————
enum class Debug{OFF,ON,};
extern Debug debugMode;
// ————————————————————————————————————————



// ——————————————————— functions ——————————
=======

enum class Debug{OFF,ON,};
extern Debug debugMode;

>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
bool DEBUG_ON();

void DEBUG_PageNum();
void DEBUG_ButtonPress();
void DEBUG_Indicators();

void DEBUG_CharSelect();

void DEBUG_Gamemode();
void DEBUG_Difficulty();
void DEBUG_Direction();
<<<<<<< HEAD
void DEBUG_ValidWord();
void DEBUG_DisplayWord(char* word);
void DEBUG_Time();
// ————————————————————————————————————————
=======
void DEBUG_Time();
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c

#endif