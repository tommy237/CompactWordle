// #pragma once
#ifndef __WORDLE_H__
#define __WORDLE_H__
#define __ASSERT_USE_STDERR

#include <Arduino.h>
#include <assert.h>
#include "buttons.h"
#include "lcd.h"
#include "word_bank.h"
#include "game_timer.h"
#include "debug.h"

// ——————————————————— enum flags —————————
enum class GameState { NONE,PLAYING,WON,LOST };
enum class LetterState {NONE,
  INCORRECT, // gray   []
  PARTIAL,   // yellow []
  MATCH,     // green  []
};
enum class Page {
  MAIN_MENU,
  GAMEMODE_MENU,
  DIFF_MENU,
  GAME_HUD,
  RESULTS_HUD,


  numPages, // total # flag
};

// regular wordle game: 6 limited guesses, unlimited time, and 5-letter words.
// unlimited guesses, limited time, and 5-8 letter words.
// get # of words correct under # of guesses in a limited time.
enum class Gamemode { REGULAR,SPEED,DEATHMATCH, 


  numGamemodes, // total # flag
};
enum class Difficulty { NORMAL,MODERATE,EXPERT,HARDCORE,


  numDiffs, // total # flag
};
// ————————————————————————————————————————


// ——————————————————— program flags ——————
extern GameState gameStatus;
extern Page pageNum;
extern Gamemode gameMode;
extern Difficulty difficulty;
// ————————————————————————————————————————


// ——————————————————— number positioning —
extern size_t guesses;  // current # of guesses in game.
extern int scrollSpeed; // speed of char scrolling when holding.
extern int charPos;     // pos of current letter of guessing word.
extern int letterPos;   // pos of alphabet selection.
extern size_t wordLength;
// ————————————————————————————————————————


// ——————————————————— array-ngements —————
extern char arrangement[];              // current letter arrangement.
extern LetterState indicator[];         // current letter indicators.
extern const char* const wordGuesses[]; // record of letter arrangement guesses.
// ————————————————————————————————————————


// ——————————————————— macros —————————————
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 8

#define MIN_ALPHABET 0x41  // A = 0x41 (65)
#define MAX_ALPHABET 0x5A  // Z = 0x5A (90)
// ————————————————————————————————————————


// ——————————————————— functions ——————————
bool WordleMode();
bool SpeedMode();
bool DeathmatchMode();

bool Normal();
bool Moderate();
bool Expert();
bool Hardcore();

void update_Screen();
void update_Status();

bool WordValid(const char guess[],const size_t wordLength);
bool CorrectWord(const char guess[],const char secret[]);
void RandomWord(char *buffer,size_t bufferSize,int wordSize);
// ————————————————————————————————————————

#endif