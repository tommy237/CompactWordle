#ifndef __DEBUG_H__
#define __DEBUG_H__

// —————— headers ————————————————————————— //
#include <Arduino.h>
#include <assert.h>
#include "buttons.h"
#include "lcd.h"
#include "wordle_prog.h"
#include "game_timer.h"
// —————— headers ————————————————————————— //



// —————— enum flags —————————————————————— //
enum class Debug{ OFF, ON, };
// —————— enum flags —————————————————————— //



// —————— public functions ————————————————— //
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
// —————— public functions ————————————————— //

#endif