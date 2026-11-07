// #pragma once
#ifndef __WORDLE_H__
#define __WORDLE_H__
#define __ASSERT_USE_STDERR

<<<<<<< HEAD
// ——————————————————— headers ————————————
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
#include <Arduino.h>
#include <assert.h>
#include "buttons.h"
#include "lcd.h"
#include "word_bank.h"
#include "game_timer.h"
#include "debug.h"
<<<<<<< HEAD
// ————————————————————————————————————————


=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c

// ——————————————————— enum flags —————————
enum class GameState { NONE,PLAYING,WON,LOST };
enum class LetterState {NONE,
  INCORRECT, // gray   []
  PARTIAL,   // yellow []
  MATCH,     // green  []
};
enum class Page {
<<<<<<< HEAD
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
=======
  MAIN_MENU,
  GAMEMODE_MENU,
  DIFF_MENU,
  GAME_HUD,
  RESULTS_HUD,


>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  numPages, // total # flag
};

// regular wordle game: 6 limited guesses, unlimited time, and 5-letter words.
// unlimited guesses, limited time, and 5-8 letter words.
// get # of words correct under # of guesses in a limited time.
<<<<<<< HEAD
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
};
enum class Difficulty { 
  NORMAL,MODERATE,EXPERT,HARDCORE,ENDLESS,
=======
enum class Gamemode { REGULAR,SPEED,DEATHMATCH, 


  numGamemodes, // total # flag
};
enum class Difficulty { NORMAL,MODERATE,EXPERT,HARDCORE,


>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  numDiffs, // total # flag
};
// ————————————————————————————————————————


<<<<<<< HEAD

// ——————————————————— program flags ——————
extern GameState game_status;
extern Page page;
extern Gamemode gameMode;
extern Difficulty difficulty;

GameState GetStatus();
Page GetPageNum();
Gamemode GetMode();
Difficulty GetDiff();

bool CheckStatus(GameState status);
bool CheckPage(Page pg);
bool CheckMode(Gamemode select);
bool CheckDiff(Difficulty diff);
// ————————————————————————————————————————



=======
// ——————————————————— program flags ——————
extern GameState gameStatus;
extern Page pageNum;
extern Gamemode gameMode;
extern Difficulty difficulty;
// ————————————————————————————————————————


>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ——————————————————— number positioning —
extern size_t guesses;  // current # of guesses in game.
extern int scrollSpeed; // speed of char scrolling when holding.
extern int charPos;     // pos of current letter of guessing word.
extern int letterPos;   // pos of alphabet selection.
extern size_t wordLength;
// ————————————————————————————————————————


<<<<<<< HEAD

=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ——————————————————— array-ngements —————
extern char arrangement[];              // current letter arrangement.
extern LetterState indicator[];         // current letter indicators.
extern const char* const wordGuesses[]; // record of letter arrangement guesses.
// ————————————————————————————————————————


<<<<<<< HEAD

=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ——————————————————— macros —————————————
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 8

#define MIN_ALPHABET 0x41  // A = 0x41 (65)
#define MAX_ALPHABET 0x5A  // Z = 0x5A (90)
// ————————————————————————————————————————


<<<<<<< HEAD

// ——————————————————— functions ——————————
void update_Screen();
void update_Status();

size_t GetMaxGuesses();
size_t GetWrongGuesses();
size_t GetCorrectGuesses();
size_t GetWordLength();
void* GetSecretWord();

void OnWait();

bool GetLastWordValid();
=======
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

>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
bool WordValid(const char guess[],const size_t wordLength);
bool CorrectWord(const char guess[],const char secret[]);
void RandomWord(char *buffer,size_t bufferSize,int wordSize);
// ————————————————————————————————————————

#endif