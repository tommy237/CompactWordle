#ifndef __STATS_H__
#define __STATS_H__

// —————— headers ————————————————————————— //
#include "wordle_prog.h"
// —————— headers ————————————————————————— //



// —————— structures ————————————————— //
struct Record {
  uint32_t id;
  Gamemode gamemode;
  Difficulty difficulty;
  GameState status;
  char last_word[10];
  uint16_t guesses;

  Record(uint32_t      _id = 0, 
         Gamemode    _mode = Gamemode::NONE, 
         Difficulty  _diff = Difficulty::NONE, 
         GameState _status = GameState::NONE, 
         uint16_t _guesses = 0,
         const char* _last = "") {
    id         = _id;
    gamemode   = _mode;
    difficulty = _diff;
    status     = _status;
    guesses    = _guesses;
    const uint8_t len = sizeof(last_word) - 1;
    strncpy(last_word, _last, len);
    last_word[len]='\0';
  }

  void copy_from(const Record& other) {
    id = other.id;
    gamemode = other.gamemode;
    difficulty = other.difficulty;
    status = other.status;
    guesses = other.guesses;
    strncpy(last_word, other.last_word, sizeof(last_word));
  }
};
// —————— structures ————————————————— //



// —————— public functions ————————————————— //
uint32_t GetTotalRounds();
void AddGameResults();
Record* GetLocalRecord(uint8_t index);
// —————— public functions ————————————————— //

#endif