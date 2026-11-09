// #pragma once
#ifndef __WORDLE_H__
#define __WORDLE_H__
#define __ASSERT_USE_STDERR

// ——————————————————— headers ———————————— //
#include <Arduino.h>
#include <assert.h>
#include "buttons.h"
#include "lcd.h"
#include "word_bank.h"
#include "game_timer.h"
#include "debug.h"
// ——————————————————— headers ———————————— //



// ——————————————————— enum flags ————————— //
enum class GameState { NONE,PLAYING,WON,LOST };
enum class LetterState {NONE,
  INCORRECT, // gray   []
  PARTIAL,   // yellow []
  MATCH,     // green  []
};
enum class Page {
  TITLE,
  MAIN_MENU,
  GAMEMODE_SELECT,
  DIFF_SELECT,
  GAME,
  RESULTS,

  GUIDE,
  STATS,
  RANKS,
  CONFIG,
  numPages, // total # flag
};

// regular wordle game: 6 limited guesses, unlimited time, and 5-letter words.
// unlimited guesses, limited time, and 5-8 letter words.
// get # of words correct under # of guesses in a limited time.
/*
    < SPEEDMODE >
    Unlimited guesses, but limited time to guess a single word.
    In endless mode, every correct guess resets the timer.
      - however, having a wrong guess will penalize your points.
    (feasible for gameplay: users can think with the allotted time; can get as many as 10+ words.)
    (feasible for leaderboard: points, guesses & cGuesses help allocate how responsive or tactful the guesses are.)

    There were ideas for an endless speedmode:
    - Achieve how many words were guessed correctly in a short time.
        (not feasible for gameplay; somewhat <3 words)
    - How fast the word is guessed correctly.
        (possibly for a different gamemode,
        although not feasible for leaderboard;
        ultimately depends on word choice, which spans for LOTS)
*/
enum class Gamemode { 
  REGULAR,SPEED,DEATHMATCH, 
  numGamemodes, // total # flag
  NONE, // null flag
};
enum class Difficulty { 
  NORMAL,MODERATE,EXPERT,HARDCORE,ENDLESS,
  numDiffs, // total # flag
  NONE, // null flag
};
// ——————————————————— enum flags ————————— //



// ——————————————————— number positioning — //
extern size_t guesses;  // current # of guesses in game.
// extern int scrollSpeed; // speed of char scrolling when holding.
// ——————————————————— number positioning — //



// ——————————————————— array-ngements —————
extern char arrangement[];              // current letter arrangement.
extern LetterState indicator[];         // current letter indicators.
extern const char* const wordGuesses[]; // record of letter arrangement guesses.
// ————————————————————————————————————————



// ——————————————————— macros ————————————— //
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 8
// ——————————————————— macros ————————————— //



// —————————————— public functions ————————— //
GameState GetStatus();
Page GetPageNum();
Gamemode GetMode();
Difficulty GetDiff();

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
bool WordValid(const char guess[],const size_t wordLength);
bool CorrectWord(const char guess[],const char secret[]);
void RandomWord(char *buffer,size_t bufferSize,int wordSize);
// —————————————— public functions ————————— //

#endif