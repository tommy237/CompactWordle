// —————— headers ————————————————————————— //
#include "leaderboard.h"
// —————— headers ————————————————————————— //

// —————— global vars ————————————————————— //
struct SecretWord { char key[10]; };
const uint8_t max_local_records = 20;
Record local_records[max_local_records];
uint32_t total_rounds = 0, 
         curr_local_rec_size = 0;
// uint32_t total_guesses = 0,
//          max_guesses = 0,
//          guesses = 0,
//          wrong_guesses = 0,
//          crrt_guesses = 0;
// —————— global vars ————————————————————— //


// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

Record* GetLocalRecords() { return local_records; }
uint32_t GetTotalRounds() { return total_rounds; }
uint8_t GetMaxLocalRecords() { return max_local_records; }

// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| DATA SAVING |—————————————————————————| //

void AddGameResults()
{
  if (CheckStatus(GameState::PLAYING)
   || CheckStatus(GameState::NONE)) return;
  total_rounds++;
  const void* sc = GetSecretWord();
  const SecretWord* sec = (SecretWord*)sc;
  const Record new_rec(total_rounds, GetMode(), GetDiff(), GetStatus(), sec->key);
  strncpy(new_rec.last_word, sec->key, sizeof(new_rec.last_word) - 1);
  const bool below_max = (curr_local_rec_size < max_local_records);
  const uint8_t max_i = (max_local_records - 1);
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
}

Record* GetLocalRecord(uint8_t i)
{
  if ((i >= curr_local_rec_size) || (i < 0)) return nullptr;
  uint32_t offset_i = ((total_rounds - curr_local_rec_size) + i) % max_local_records;
  return &local_records[offset_i];
}

// |————| DATA SAVING |—————————————————————————| //
// |————————————————————————————————————————————| //