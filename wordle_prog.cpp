// ——————————————————— headers ———————————— //
#include "wordle_prog.h"
// ——————————————————— headers ———————————— //


// —————— program flags ——————————————————— //
Page page=Page::TITLE;               // current page
GameState game_status=GameState::NONE;       // current result
Gamemode gameMode=Gamemode::REGULAR;
Difficulty difficulty=Difficulty::NORMAL;
// —————— program flags ——————————————————— //



// —————— number positioning —————————————— //
int8_t charPos=0;             // pos of current letter of guessing word.
int8_t letterPos=0;           // pos of alphabet selection.
unsigned long lockMs=0;
unsigned int lastDelay=0;
bool guess_valid=false;

uint8_t word_len=5;         // current secret word length
struct SecretWord {
  char key[10];
};
static SecretWord sec;
char arrangement[10]={};      // current letter arrangement
LetterState indicator[10]={}; // current letter indicators
LetterState organizer[26][8]={};
// —————— number positioning —————————————— //



// —————— data storage ————————————————————— //
const char* const wordGuesses[] PROGMEM={}; // current words already guessed.
const char ALPHABET_NUMBER[37]="ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
const uint8_t ALPHASIZE=sizeof(ALPHABET_NUMBER)-10;

size_t guesses=0;
size_t max_guesses=0;
size_t wrong_guesses=0; // wrong guesses;
size_t crrt_guesses=0; // correct guesses;
size_t crrt_needed=0;
size_t num_words=word_packs[0].total;
// —————— data storage ————————————————————— //



// ————— function headers ————————————————— //
void goto_MainMenu();
void goto_GamemodePage();
// ————— function headers ————————————————— //



// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

// ———————— FLAG READING ———————— //
GameState GetStatus() { return game_status; }
Page GetPageNum() { return page; }
Gamemode GetMode() { return gameMode; }
Difficulty GetDiff() { return difficulty; }
// ———————— FLAG READING ———————— //


// ———————— SIZE READING ———————— //
size_t GetTotalWords() { return num_words; }
size_t GetTotalGuesses() { return guesses; }
size_t GetMaxGuesses() { return max_guesses; }
size_t GetWrongGuesses() { return wrong_guesses; }
size_t GetCorrectGuesses() { return crrt_guesses; }
uint8_t GetWordLength() { return word_len; }
// ———————— SIZE READING ———————— //


// ———————— POSITIONAL READING ———————— //
int8_t GetHorizontalPos() { return charPos; }
int8_t GetVerticalPos() { return letterPos; }
// ———————— POSITIONAL READING ———————— //


// ———————— CONDITIONAL READING ———————— //
bool CheckStatus(GameState status) { return game_status==status; }
bool CheckPage(Page pg) { return page==pg; }
bool CheckMode(Gamemode select) { return gameMode==select; }
bool CheckDiff(Difficulty diff) { return difficulty==diff; }
bool GetLastWordValid() { return guess_valid; }
// ———————— CONDITIONAL READING ———————— //

void* GetSecretWord() { return (void*)&sec; }

// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| RESET FUNCTIONS |—————————————————————| //
void ResetPos() {
  const bool isPlaying=(game_status==GameState::PLAYING);
  letterPos=isPlaying?ALPHASIZE:0;
  charPos=0;
}
void ClrLocalStats() {
  wrong_guesses=0;
  crrt_guesses=0;
  guesses=0;
}
void ClrArrange() { for (int c=0;c<(word_len+1);c++) { arrangement[c]='\0'; } }
void ClrIndicators() { for (int c=0;c<(word_len+1);c++) { indicator[c]=LetterState::NONE; } }
void ClrLetters() {
  for (int c=0;c<(word_len+1);c++) {
    arrangement[c]='\0';
    indicator[c]=LetterState::NONE;
  }
}
// |————| RESET FUNCTIONS |—————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| PROGRAM FUNCTIONS |———————————————————| //

// Chooses a random word from the word bank depending on the # of letters.
void RandomWord() {
  char *secret_word=sec.key;
  const size_t b_size=sizeof(sec.key),
               ofs=(b_size-1);
  const WordPack& pack=word_packs[word_len-5];
  num_words=pack.total;
  size_t rand_i=random(num_words);
  strncpy_P(secret_word,pack.bank[rand_i],ofs);
  secret_word[ofs]='\0';
  DEBUG_DisplayWord(secret_word); // comment out if not needed
}

// Checks to see if the word exists from the word bank.
bool WordValid(const char* guess) {
  const WordPack& pack=word_packs[word_len-5];
  for (size_t i=0;i<pack.total;i++) {
    const char* cur=(pack.bank+(i*(pack.leng+1)));
    if (strcmp_P(guess,cur)==0) return true;
  }
  return false;
}

// Checks for the matching letters and sequence with the secret word.
// Also updates the indicators to help signal letter correctness.
bool CorrectWord(const char guess[]) {
  const char *secret=sec.key;
  const uint8_t g_leng=strlen(guess),
                s_leng=strlen(secret);
  if (g_leng!=s_leng) return false;
  uint8_t ltrs_correct=0;
  for (uint8_t i=0;i<word_len;i++) {
    const char& g_char=guess[i],
                s_char=secret[i];
    const bool match=(g_char==s_char),
               still_found=(strchr(secret,g_char)!=NULL);
    indicator[i]=                   match?LetterState::MATCH:
                ((!match)&&(still_found))?LetterState::PARTIAL:
                                          LetterState::INCORRECT;
    if (match) ltrs_correct++;
  }
  return (ltrs_correct==s_leng);
}
// |————| PROGRAM FUNCTIONS |———————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| SCREEN LOCKING |——————————————————————| //
void UpdateLockTime(int del) {
  lockMs=millis();
  lastDelay=del;
}

void AuthorPrompt(int prompt_delay) {
  if (!OnPopup()) { 
    SetDisplayFlag(true);
    UpdateLockTime(prompt_delay);
  }
}

void SetLock(int lock_delay) {
  if (!OnLocked()) {
    LockButtons(true);
    UpdateLockTime(lock_delay);
  }
}

void OnWait() {
  const unsigned long ms=millis(),
                      diffMs=(ms-lockMs);
  if ((diffMs>(lastDelay*1E3))) {
    if (OnPopup()) { SetDisplayFlag(false); } // locking the game
    else if (OnLocked()) { LockButtons(false); } // locking the buttons
  }
}
// |————| SCREEN LOCKING |——————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| BUTTON SEEKING |——————————————————————| //
void SeekLetter() {
  if (!NoResp()) {
    const int8_t ud=(-UpDwn()),
                 lr=LftRgt();
    // DEBUG_Direction();

    // USER presses UP or DOWN.
    if (ud!=0) { 
      const int i=(letterPos+(ud*GetScrollSpeed()));
      const bool below=(i<0),
                 above=(i>=(ALPHASIZE-1));
      letterPos=below?(ALPHASIZE-2):above?0:i;
      arrangement[charPos]=ALPHABET_NUMBER[letterPos];
    }
    
    // USER presses LEFT or RIGHT.
    else if (lr!=0) { 
      uint8_t n_i=(charPos+lr);
      const bool below=(n_i<0),
                 above=(n_i>=word_len);
      n_i=below?(word_len-1):above?0:n_i;
      const char& r_char=arrangement[n_i];
      uint8_t n_char=(r_char-'A')+1;
      if (r_char=='\0') n_char=ALPHASIZE;
      letterPos=(n_char-1);
      charPos=n_i;
    }
  }
}
// |————| BUTTON SEEKING |——————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| PAGE INDEXING |———————————————————————| //
void ToPage(Page pg) {
  page=pg;
  ResetPos();
}

// |——————————————————————————————| //
// |————| MAIN GAME UI |——————————| //
void GameWin() {
  game_status=GameState::WON;
  StopTimer();
  if (CheckDiff(Difficulty::ENDLESS)) {
    
  }
  AuthorPrompt(7);
  ToPage(Page::RESULTS);
  
}

void GameLose() {
  game_status=GameState::LOST;
  AuthorPrompt(7);
  ToPage(Page::RESULTS);
}

bool Confirmation(){
  const bool endless=CheckDiff(Difficulty::ENDLESS);

  // ———————— D E A T H M A T C H ———————— //
  if (CheckMode(Gamemode::DEATHMATCH)) {
    crrt_guesses++;
    if (crrt_guesses<=crrt_needed) return false;
    return true;
  }
  // ———————— D E A T H M A T C H ———————— //
  
  // ———————— S P E E D ———————— //
  else if (CheckMode(Gamemode::SPEED)) { 
    if (endless) {
      RandomWord();
      const uint8_t mins=(crrt_guesses>10)?2:
                         (crrt_guesses>5)?3:
                         (crrt_guesses>3)?4:5;
      UpdateTimer(mins-GetMinutes(),
                  0-GetSeconds());
    }
    return !endless; // must return false, endless mode
  }
  // ———————— S P E E D ———————— //
  
  // ———————— W O R D L E ———————— //
  else {
    if (endless) {
      RandomWord();
      max_guesses=(crrt_guesses>10)?3:
                  (crrt_guesses>5)?4:
                  (crrt_guesses>3)?5:6;
    }
    return !endless; // must return false; endless mode
  }
  // ———————— W O R D L E ———————— //

  return false;
}

void PlayGame() {
  page=Page::GAME;
  if (CheckStatus(GameState::NONE)) {
    game_status=GameState::PLAYING;
    ClrLocalStats();
    const uint8_t diff_i=static_cast<uint8_t>(difficulty);

    // ———————— D E A T H M A T C H ———————— //
    if (CheckMode(Gamemode::DEATHMATCH)) {
      const uint8_t guess_stash[]={20,18,14,10}; // guesses stash by difficulty
      const uint8_t goal_stash[]={5,6,7,8};    // correct guesses goal by difficulty
      max_guesses=guess_stash[diff_i];
      crrt_needed=goal_stash[diff_i];
      crrt_guesses=0;
      const uint8_t m_stash[]={10,7,6,5};  // minutes by difficulty
      const uint8_t s_stash[]={0,30,15,0}; // seconds by difficulty
      UpdateTimer(m_stash[diff_i],s_stash[diff_i]);
    }
    // ———————— D E A T H M A T C H ———————— //
    
    // ———————— S P E E D ———————— //
    else if (CheckMode(Gamemode::SPEED)) {
      const uint8_t m_stash[]={10,5,3,2,5}; // minutes by difficulty
      UpdateTimer(m_stash[diff_i],0);
    }
    // ———————— S P E E D ———————— //
    
    // ———————— W O R D L E ———————— //
    else {
      max_guesses=6;
      const uint8_t len_stash[]={5,6,6,7,5}; // length by difficulty
      word_len=len_stash[diff_i];
    }
    // ———————— W O R D L E ———————— //

    RandomWord();
    AuthorPrompt(5);
    ClrLetters();
    ResetPos();
    if (!CheckMode(Gamemode::REGULAR)) {
      PlayTimer();
    }
  }
}

void update_Status() {
  if (CheckPage(Page::RESULTS)) return;
  if (!CheckStatus(GameState::PLAYING)) return;
  if (!CheckMode(Gamemode::REGULAR)) {
    if (NoTime()) {
      GameLose();
    }
  } else {
    if (wrong_guesses>=6) {
      GameLose();
    }
  }
}

void update_GameHud() {
  SeekLetter();
  // DEBUG_CharSelect();
  if (!Cnfrm()) return;
  // DEBUG_ValidWord(); // comment out if not needed
  guess_valid=WordValid(arrangement);
  if (!guess_valid) {
    SetLock(1);
    return;
  }
  const bool correct=CorrectWord(arrangement);
  DEBUG_Indicators(); // comment out if not needed
  guesses++;
  SetLock(1);
  if (correct) {
    crrt_guesses++;
    if (Confirmation()) { GameWin(); }
    wrong_guesses=0;
  } else {
    wrong_guesses++;
  }
  ClrLetters();
  ResetPos();
}

void ExitToMenu(const uint8_t num) {
  goto_MainMenu();
  charPos=num;
}
// |————| MAIN GAME UI |——————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| DIFFICULTY PAGE |———————| //
void goto_DiffPage() { ToPage(Page::DIFF_SELECT); }

void update_DiffPage() {
  const uint8_t n_diffs=static_cast<uint8_t>(Difficulty::numDiffs);
  Scrolling(&charPos,n_diffs,-UpDwn());
  // if (LftRgt()==0) { DEBUG_Difficulty(); } // comment out if not needed
  if (LftRgt()==-1) goto_GamemodePage();
  if (Cnfrm()) {
    difficulty=static_cast<Difficulty>(charPos);
    PlayGame();
  }
}
// |————| DIFFICULTY PAGE |———————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| GAMEMODE PAGE |—————————| //
void goto_GamemodePage() { ToPage(Page::GAMEMODE_SELECT); }

void update_GamemodePage() {
  const uint8_t n_modes=static_cast<uint8_t>(Gamemode::numGamemodes);
  Scrolling(&charPos,n_modes,-UpDwn());
  // if (LftRgt()==0) { DEBUG_Gamemode(); } // comment out if not needed
  if (LftRgt()==-1) goto_MainMenu();
  if (Cnfrm()) {
    // --- applying pre-stats before the game section --- //
    gameMode=static_cast<Gamemode>(charPos);
    switch(gameMode) {
      case Gamemode::REGULAR: break;
      case Gamemode::SPEED: break;
      case Gamemode::DEATHMATCH: break;
    }
    // -------------------------------------------------- //
    goto_DiffPage();
  }
}
// |————| GAMEMODE PAGE |—————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| GUIDE PAGE |————————————| //
void goto_GuidePage() { ToPage(Page::GUIDE); }

void update_Guide() {
  Scrolling(&charPos,NUM_GUIDE_PAGES,LftRgt());
  if (Cnfrm()) { ExitToMenu(1); }
}
// |————| GUIDE PAGE |————————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| STATS PAGE |————————————| //
void goto_StatsPage() { ToPage(Page::STATS); }

void update_Stats() {
  if (Cnfrm()) { ExitToMenu(2); }
}
// |————| STATS PAGE |————————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| RANKS PAGE |————————————| //
void goto_RanksPage() { ToPage(Page::RANKS); }

void update_Ranks() {
  if (Cnfrm()) { ExitToMenu(3); }
}
// |————| RANKS PAGE |————————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| CONFIG PAGE |———————————| //
void goto_ConfigPage() { ToPage(Page::CONFIG); }

void update_Config() {
  if (Cnfrm()) { ExitToMenu(4); }
}
// |————| CONFIG PAGE |———————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| MAIN MENU |—————————————| //
void goto_MainMenu() {
  game_status=GameState::NONE;
  gameMode=Gamemode::REGULAR;
  ToPage(Page::MAIN_MENU);
  guesses=0;
}

void show_TitlePage() {
  if (Cnfrm()) {
    goto_MainMenu();
  }
}

void update_MainMenu() {
  Scrolling(&charPos,5,-UpDwn());
  if (LftRgt()==-1) { ToPage(Page::TITLE); };
  if (Cnfrm()) {
    switch (charPos) {
      case 0: goto_GamemodePage(); break;
      case 1: goto_GuidePage(); break;
      case 2: goto_StatsPage(); break;
      case 3: goto_RanksPage(); break;
      case 4: goto_ConfigPage(); break;
      default: break;
    }
  }
}
// |————| MAIN MENU |—————————————| //
// |——————————————————————————————| //



// |——————————————————————————————| //
// |————| SCREEN UPDATE |—————————| //
void print_Results() {
  ResetPos();
  int ud=UpDwn();
  int lr=LftRgt();
  Scrolling(&letterPos,5,UpDwn()); // up/down    cursor
  Scrolling(&charPos,5,LftRgt());    // left/right cursor
  if (Cnfrm()) {
    goto_MainMenu();
  }
}

void update_Screen() {
  // DEBUG_PageNum(); // comment out if not needed
  if (!OnPopup()) {
    switch (page) {
      // Main title //
      case Page::TITLE: show_TitlePage(); break;

      // Menu directory //
      case Page::MAIN_MENU: update_MainMenu(); break;
      case Page::GUIDE: update_Guide(); break;
      case Page::STATS: update_Stats(); break;
      case Page::RANKS: update_Ranks(); break;
      case Page::CONFIG: update_Config(); break;

      // Main game directory //
      case Page::GAMEMODE_SELECT: update_GamemodePage(); break;
      case Page::DIFF_SELECT: update_DiffPage(); break;
      case Page::GAME: update_GameHud(); break;
      case Page::RESULTS: print_Results(); break;
      
      default: break;
    }
  }
}
// |————| SCREEN UPDATE |—————————| //
// |——————————————————————————————| //
// |————| PAGE INDEXING |———————————————————————| //
// |————————————————————————————————————————————| //