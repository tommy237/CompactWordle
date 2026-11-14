#ifndef __STATES_H__
#define __STATES_H__

// —————— enum flags —————————————————————— //
enum class GameState { NONE, PLAYING, WON, LOST };
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
// —————— enum flags —————————————————————— //

#endif