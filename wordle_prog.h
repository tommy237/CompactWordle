// #pragma once
#ifndef __WORDLE_H__
#define __WORDLE_H__
#define __ASSERT_USE_STDERR

// —————— headers ————————————————————————— //
#include <Arduino.h>
#include <assert.h>
#include "states.h"
#include "buttons.h"
#include "lcd.h"
#include "word_bank.h"
#include "game_timer.h"
#include "leaderboard.h"
#include "debug.h"
// —————— headers ————————————————————————— //



// —————— number positioning —————————————— //
extern size_t guesses;  // current # of guesses in game.
// extern int scrollSpeed; // speed of char scrolling when holding.
// —————— number positioning —————————————— //



// —————— array-ngements —————————————————— //
extern char arrangement[];              // current letter arrangement.
extern LetterState indicator[];         // current letter indicators.
extern const char* const wordGuesses[]; // record of letter arrangement guesses.
// —————— array-ngements —————————————————— //



// —————— macros —————————————————————————— //
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 8
// —————— macros —————————————————————————— //



// —————— public functions ———————————————— //
GameState GetStatus();
Page GetPageNum();
Gamemode GetMode();
Difficulty GetDiff();
LetterState GetLetterStatus(const uint8_t i);

int8_t GetHorizontalPos();
int8_t GetVerticalPos();
size_t GetMaxGuesses();
size_t GetWrongGuesses();
size_t GetCorrectGuesses();
uint8_t GetWordLength();
void* GetSecretWord();

bool CheckStatus(GameState status);
bool CheckPage(Page pg);
bool CheckMode(Gamemode select);
bool CheckDiff(Difficulty diff);

void update_Screen();
void update_Status();
void OnWait();
bool GetLastWordValid();
// —————— public functions ———————————————— //

#endif