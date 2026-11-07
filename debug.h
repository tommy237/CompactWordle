#ifndef __DEBUG_H__
#define __DEBUG_H__

// ——————————————————— headers ————————————
#include <Arduino.h>
#include "buttons.h"
#include "lcd.h"
#include "wordle_prog.h"
#include "game_timer.h"
// ————————————————————————————————————————


// ——————————————————— enum flags —————————
enum class Debug{OFF,ON,};
extern Debug debugMode;
// ————————————————————————————————————————



// ——————————————————— functions ——————————
bool DEBUG_ON();

void DEBUG_PageNum();
void DEBUG_ButtonPress();
void DEBUG_Indicators();

void DEBUG_CharSelect();

void DEBUG_Gamemode();
void DEBUG_Difficulty();
void DEBUG_Direction();
void DEBUG_ValidWord();
void DEBUG_DisplayWord(char* word);
void DEBUG_Time();
// ————————————————————————————————————————

#endif