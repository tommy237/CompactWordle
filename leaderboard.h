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
  const char* last_word;
  uint16_t guesses;

  void copy_from(const Record& other) {
    id = other.id;
    gamemode = other.gamemode;
    difficulty = other.difficulty;
    status = other.status;
    guesses = other.guesses;
    last_word = other.last_word;
  }
};

enum class EntryType {
  _INT,
  _STRING,
};

struct Entry {
  const char* key;
  EntryType type;
  union {
    uint32_t value;
    const char* str;
  } data;
};
// —————— structures ————————————————— //



// —————— public functions ————————————————— //
uint32_t GetTotalGuesses();
uint32_t GetGuessLimit();
uint32_t GetGuessGoal();
uint32_t GetLocalGuesses();
uint32_t GetCorrectGuesses();
uint32_t GetWrongGuesses();
uint8_t GetMaxLocalRecords();

void ClearGuesses();
void SetGuessLimit(uint32_t limit);
void SetGuessGoal(uint32_t goal);
void AddGuessClearWrong(bool correct);
void AddGuess(bool correct);

Entry MakeEntry(const char* key, uint32_t value);
Entry MakeEntry(const char* key, const char* str);

uint32_t GetTotalRounds();
void AddGameResults();

Record MakeRecord(
  uint32_t id, 
  Gamemode mode, 
  Difficulty diff, 
  GameState status, 
  const char* last_word, 
  uint16_t guesses);
Record* GetLocalRecord(uint8_t index);
// —————— public functions ————————————————— //

#endif