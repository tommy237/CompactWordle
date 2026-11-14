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
         curr_local_rec_size = 0;
uint32_t total_guesses = 0,
         max_guesses = 0,
         guesses = 0,
         wrong_guesses = 0,
         crrt_guesses = 0;
// —————— global vars ————————————————————— //


// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

uint32_t GetTotalGuesses() { return total_guesses; }
uint32_t GetGuessLimit() { return max_guesses; }
uint32_t GetLocalGuesses() { return guesses; }
uint32_t GetCorrectGuesses() { return crrt_guesses; }
uint32_t GetGuessGoal() { return crrt_needed; }
uint32_t GetWrongGuesses() { return wrong_guesses; }

Record* GetLocalRecords() { return local_records; }
uint32_t GetTotalRounds() { return total_rounds; }
uint8_t GetMaxLocalRecords() { return MAX_LOCAL_RECORDS; }

// |————| RETURN FUNCTIONS |————————————————————| //
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
  ClearGuesses();
}

void AddGuess(bool correct) {
  uint32_t counter = correct ? &crrt_guesses : &wrong_guesses;
  guesses++;
  counter++;
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
  const void* sc = GetSecretWord();
  const SecretWord* sec = (SecretWord*)sc;
  const Record new_rec(total_rounds, GetMode(), GetDiff(), GetStatus(), sec->key, GetLocalGuesses());
  strncpy(new_rec.last_word, sec->key, sizeof(new_rec.last_word) - 1);
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

Record* GetLocalRecord(uint8_t i)
{
  if ((i >= curr_local_rec_size) || (i < 0)) return nullptr;
  uint32_t offset_i = ((total_rounds - curr_local_rec_size) + i) % MAX_LOCAL_RECORDS;
  return &local_records[offset_i];
}

// |————| DATA SAVING |—————————————————————————| //
// |————————————————————————————————————————————| //