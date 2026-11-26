// —————— headers ————————————————————————— //
#include "wordle_prog.h"
#include "leaderboard.h"
// —————— headers ————————————————————————— //



// —————— settings ———————————————————————— //
#define MAX_LOCAL_RECORDS 20
// —————— settings ———————————————————————— //


// —————— global vars ————————————————————— //
struct SecretWord { char key[10]; };
Record local_records[MAX_LOCAL_RECORDS];

uint32_t total_rounds = 0,
         total_endless = 0,
         total_won = 0,
         curr_local_rec_size = 0;

uint32_t total_guesses = 0,
         total_crrt = 0,
         max_guesses = 0,
         guesses = 0,
         wrong_guesses = 0,
         crrt_guesses = 0,
         crrt_needed = 0;

// —————— global vars ————————————————————— //


// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

uint32_t GetTotalGuesses()   { return total_guesses; }
uint32_t GetTotalCorrect()   { return total_crrt; }
uint32_t GetLocalGuesses()   { return guesses; }
uint32_t GetGuessLimit()     { return max_guesses; }
uint32_t GetGuessGoal()      { return crrt_needed; }
uint32_t GetCorrectGuesses() { return crrt_guesses; }
uint32_t GetWrongGuesses()   { return wrong_guesses; }

uint32_t GetTotalRounds()  { return total_rounds; }
uint32_t GetTotalEndless() { return total_endless; }
uint32_t GetTotalWon()     { return total_won; }

uint8_t GetMaxLocalRecords() { return MAX_LOCAL_RECORDS; }
Record* GetLocalRecords()    { return local_records; }

// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| ENTRY SAVING |————————————————————————| //
Entry EntryModify(Entry* entry, const char* key, EntryType type) {
  entry->type = type;
  entry->key = key;
  return *entry;
}

Entry MakeEntry(const char* key, uint32_t value) {
  Entry e = EntryModify(&e, key, EntryType::_INT);
  e.data.value = value;
  return e;
}

Entry MakeEntry(const char* key, const char* str) {
  Entry e = EntryModify(&e, key, EntryType::_STRING);
  e.data.str = str;
  return e;
}
// |————| ENTRY SAVING |————————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| ROUND RECORDING |—————————————————————| //

Record MakeRecord(uint32_t   id = 0, 
               Gamemode    mode = Gamemode::NONE, 
               Difficulty  diff = Difficulty::NONE, 
               GameState status = GameState::NONE, 
               const char* last_word = "",
               uint16_t guesses = 0) {
  Record rec;
  rec.id         = id;
  rec.gamemode   = mode;
  rec.difficulty = diff;
  rec.status     = status;
  rec.last_word  = last_word;
  rec.guesses    = guesses;
  return rec;
}

Record* GetLocalRecord(uint8_t i)
{
  if ((i >= curr_local_rec_size) || (i < 0)) return nullptr;
  uint32_t offset_i = ((total_rounds - curr_local_rec_size) + i) % MAX_LOCAL_RECORDS;
  return &local_records[offset_i];
}

// |————| ROUND RECORDING |—————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| DATA SAVING |—————————————————————————| //

void ClearGuesses() {
  guesses = 0;
  crrt_guesses = 0;
  wrong_guesses = 0;
}

void SetGuessLimit(uint32_t limit) { max_guesses = limit; }
void SetGuessGoal(uint32_t goal) { crrt_needed = goal; }

void TallyGuesses()
{
  total_guesses += guesses;
  total_crrt += crrt_guesses;
  ClearGuesses();
}

void AddGuess(bool correct) {
  uint32_t* counter = correct ? &crrt_guesses : &wrong_guesses;
  guesses++;
  (*counter)++;
}

void AddGuessClearWrong(bool correct) {
  AddGuess(correct);
  if (correct) wrong_guesses = 0;
}

void AddGameResults()
{
  if (CheckStatus(GameState::PLAYING)
   || CheckStatus(GameState::NONE)) return;

  total_rounds++;
  const bool is_endless = CheckDiff(Difficulty::ENDLESS);
  if (is_endless) total_endless++;
  else if (CheckStatus(GameState::WON)) total_won++;

  const void* sc = GetSecretWord();
  const SecretWord* sec = (SecretWord*)sc;
  Record new_rec = MakeRecord(
    total_rounds, 
    GetMode(), 
    GetDiff(), 
    GetStatus(), 
    const_cast<char*>(sec->key), 
    guesses);
  // strncpy(new_rec.last_word, sec->key, sizeof(new_rec.last_word) - 1);
  const bool below_max = (curr_local_rec_size < MAX_LOCAL_RECORDS);
  const uint8_t max_i = (MAX_LOCAL_RECORDS - 1);
  // when all records are pushed down in the array, 
  // this allows an opening for the new record.
  if (!below_max) {
    for (uint8_t i = 0; i < max_i; i++)
      local_records[i].copy_from(local_records[i + 1]);
  }
  local_records[below_max ? curr_local_rec_size : max_i].copy_from(new_rec);
  // curr_local_rec_size starts at zero, 
  // makes sure local_records[] is completely filled with new records.
  if (below_max) curr_local_rec_size++;
  TallyGuesses();
}

// |————| DATA SAVING |—————————————————————————| //
// |————————————————————————————————————————————| //