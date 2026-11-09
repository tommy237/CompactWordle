// ——————————————————— headers ———————————— //
#include "leaderboard.h"
// ——————————————————— headers ———————————— //

struct SecretWord {char key[10];};

const uint8_t max_local=20;
Record local_records[max_local];
uint32_t total_rounds=0,
         curr_local=0;


// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

Record* GetLocalRecords() { return local_records; }
uint32_t GetTotalRounds() { return total_rounds; }
uint8_t GetMaxLocalRecords() { return max_local; }

// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| DATA SAVING |—————————————————————————| //

void AddGameResults() {
  if (CheckStatus(GameState::PLAYING)
    ||CheckStatus(GameState::NONE)) return;
  total_rounds++;
  const void* sc=GetSecretWord();
  const SecretWord* sec=(SecretWord*)sc;
  const Record new_rec(total_rounds,GetMode(),GetDiff(),GetStatus(),sec->key);
  strncpy(new_rec.last_word,sec->key,sizeof(new_rec.last_word)-1);
  const bool below_max=(curr_local<max_local);
  const uint8_t max_i=(max_local-1);
  if (!below_max) {
    for (uint8_t i=0;i<max_i;i++)
      local_records[i].copy_from(local_records[i+1]);
  }
  local_records[below_max?curr_local:max_i].copy_from(new_rec);
  if (below_max) curr_local++;
}

Record* GetLocalRecord(uint8_t i) {
  if ((i>=curr_local)||(i<0)) return nullptr;
  uint32_t offset_i=((total_rounds-curr_local)+i)%max_local;
  return &local_records[offset_i];
}

// |————| DATA SAVING |—————————————————————————| //
// |————————————————————————————————————————————| //