#include "wordle_prog.h"

<<<<<<< HEAD


const char* const wordGuesses[] PROGMEM={}; // current words already guessed.
const char ALPHABET[27]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const size_t ALPHASIZE=sizeof(ALPHABET);



// ——————————————————— program flags ——————
Page page=Page::TITLE;               // current page
GameState game_status=GameState::NONE;       // current result
=======
const char* const wordGuesses[] PROGMEM={}; // current words already guessed.

// ——————————————————— program flags ——————
Page pageNum=Page::MAIN_MENU;               // current page
GameState gameStatus=GameState::NONE;       // current result
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
Gamemode gameMode=Gamemode::REGULAR;
Difficulty difficulty=Difficulty::NORMAL;
// ————————————————————————————————————————

<<<<<<< HEAD


// ——————————————————— number positioning —
int charPos=0;             // pos of current letter of guessing word (and main cursor of selection)
int letterPos=0;           // pos of alphabet selection
unsigned long lockMs=0;
int lastDelay=0;
bool guess_valid=false;

// ——————— REGULAR ———————————————————————————
uint8_t word_len=5;         // current secret word length
struct SecretWord {
  char key[10];
};
static SecretWord sec;
char arrangement[10]={};      // current letter arrangement
LetterState indicator[10]={}; // current letter indicators
LetterState organizer[26][8]={};
size_t guesses=0;
size_t max_guesses=0;
size_t wrong_guesses=0; // wrong guesses;
size_t crrt_guesses=0; // correct guesses;
size_t crrt_needed=0;
size_t num_words=word_packs[0].total;

// ————————————————————————————————————————



=======
// ——————————————————— number positioning —
int charPos=0;             // pos of current letter of guessing word (and main cursor of selection)
int letterPos=0;           // pos of alphabet selection

// ——————— REGULAR ———————————————————————————
size_t wordLength=5;         // current secret word length
char secretWord[10];
char arrangement[6]={};      // current letter arrangement
LetterState indicator[6]={}; // current letter indicators
size_t guesses=0;
size_t totalWords=WC5;

// ——————— SPEED —————————————————————
size_t points=0;
size_t wGuesses=0; // wrongGuesses;

// ——————— DEATHMATCH —————————————————————
size_t guessesLeft=0;
size_t cGuessesNeeded=0;
size_t cGuesses=0;
// ————————————————————————————————————————

>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ——————————————————— function headers ———
void goto_MainMenu();
void goto_GamemodePage();
// ————————————————————————————————————————

<<<<<<< HEAD


// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //
void* GetSecretWord() { return (void*)&sec; }

size_t GetTotalWords() { return num_words; }
size_t GetTotalGuesses() { return guesses; }
size_t GetMaxGuesses() { return max_guesses; }
size_t GetWrongGuesses() { return wrong_guesses; }
size_t GetCorrectGuesses() { return crrt_guesses; }

uint8_t GetWordLength() { return word_len; }

bool CheckStatus(GameState status) { return game_status==status; }
bool CheckPage(Page pg) { return page==pg; }
bool CheckMode(Gamemode select) { return gameMode==select; }
bool CheckDiff(Difficulty diff) { return difficulty==diff; }
bool GetLastWordValid() { return guess_valid; }

GameState GetStatus() { return game_status; }
Page GetPageNum() { return page; }
Gamemode GetMode() { return gameMode; }
Difficulty GetDiff() { return difficulty; }
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
=======
// Chooses a random word from the word bank depending on the # of letters.
void RandomWord(char *buffer,size_t bufferSize,int wordSize) {
  switch (wordSize) {
    case 7: totalWords=WC7; break;
    case 6: totalWords=WC6; break;
    default: totalWords=WC5; break;
  }
  size_t rIndex=random(totalWords);
  switch (wordSize) {
    case 7: strncpy_P(buffer,words7[rIndex],bufferSize-1); break;
    case 6: strncpy_P(buffer,words6[rIndex],bufferSize-1); break;
    default: strncpy_P(buffer,words5[rIndex],bufferSize-1); break;
  }
  buffer[bufferSize-1]='\0';
}

// Checks to see if the word exists from the word bank.
bool WordValid(const char guess[]) {
  bool isValid=false;
  for (size_t i=0;i<totalWords;i++) {
    char currentWord[wordLength+1];
    // strncpy_P(currentWord,words5[i],sizeof(currentWord)-1);
    switch (wordLength) {
      case 7: strncpy_P(currentWord,words7[i],sizeof(currentWord)-1); break;
      case 6: strncpy_P(currentWord,words6[i],sizeof(currentWord)-1); break;
      default: strncpy_P(currentWord,words5[i],sizeof(currentWord)-1); break;
    }
    currentWord[sizeof(currentWord)-1]='\0';
    size_t numLetters=strlen(currentWord)+1;
    if (numLetters!=(wordLength+1)) continue;
    isValid=((strcmp(guess,currentWord))==0);
    if (isValid) break;
  }
  return isValid;
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
}

// Checks for the matching letters and sequence with the secret word.
// Also updates the indicators to help signal letter correctness.
<<<<<<< HEAD
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
      const int i=(letterPos+(ud*scrollSpeed));
      const bool below=(i<0),
                 above=(i>=(ALPHASIZE-1));
      letterPos=below?(ALPHASIZE-2):above?0:i;
      arrangement[charPos]=ALPHABET[letterPos];
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

=======
bool CorrectWord(const char guess[],const char secret[]) {
  const size_t gLen=strlen(guess);
  const size_t sLen=strlen(secret);
  if (gLen!=sLen) return false;
  size_t lettersCorrect=0;
  for (size_t i=0;i<wordLength;i++) {
    const char g=guess[i];
    const char sc=secret[i];
    if (g==sc) {
      indicator[i]=LetterState::MATCH;
      lettersCorrect++;
    } else if (g!=sc && strchr(secret,g)!=NULL) {
      indicator[i]=LetterState::PARTIAL;
    } else {
      indicator[i]=LetterState::INCORRECT;
    }
  }
  return (lettersCorrect==sLen);
}

void ResetPos() {
  letterPos=0;
  charPos=0;
}

void ClrArrange() { for (int c=0;c<(wordLength+1);c++) { arrangement[c]='\0'; } }
void ClrIndicators() { for (int c=0;c<(wordLength+1);c++) { indicator[c]=LetterState::NONE; } }
void ClrLetters() {
  ClrArrange();
  ClrIndicators();
}

// ————————————————————————————————————————————————
// ———————————————— conditional flags —————————————
// ————————————————————————————————————————————————

bool Won() { return gameStatus==GameState::WON; }
bool Loss() { return gameStatus==GameState::LOST; }
bool IsPlaying() { return gameStatus==GameState::PLAYING; }
bool Idle() { return gameStatus==GameState::NONE; }

bool WordleMode() { return gameMode==Gamemode::REGULAR; }
bool SpeedMode() { return gameMode==Gamemode::SPEED; }
bool DeathmatchMode() { return gameMode==Gamemode::DEATHMATCH; }

bool Normal() { return difficulty==Difficulty::NORMAL; }
bool Moderate() { return difficulty==Difficulty::MODERATE; }
bool Expert() { return difficulty==Difficulty::EXPERT; }
bool Hardcore() { return difficulty==Difficulty::HARDCORE; }

// ————————————————————————————————————————————————
// ——————————————————— button seeking —————————————
// ————————————————————————————————————————————————

void SeekLetter() {
  if (!NoResp()) {
    int alphaRange=(MAX_ALPHABET-MIN_ALPHABET);
    int ud=(-UpDwn());
    int lr=LftRgt();
    size_t guessSize=LENG(arrangement);
    // DEBUG_Direction();
    if (ud!=0) { // USER presses UP or DOWN.
      int index=(letterPos+(ud*scrollSpeed)); // adds -1 or +1 to current letter pos
      letterPos=(index<=0)?(alphaRange+1):(index>alphaRange+1)?1:index; // adds -1 or +1, from 1 to 26.
      arrangement[charPos]=(letterPos-1)+MIN_ALPHABET; // selects the char from the letterPos.
    } else if (lr!=0) { // USER presses LEFT or RIGHT.
      int offset=(charPos+lr);
      int nextIndex=((offset<0)?(wordLength-1):offset)%wordLength; // safeguard index from 0 to # of letters in current word.
      char rawLetter=arrangement[nextIndex];
      int nextLetter=((rawLetter)-MIN_ALPHABET)+1;
      if (rawLetter=='\0') {nextLetter=0;}
      letterPos=nextLetter; // grabs existing
      charPos=nextIndex;
    }
  }
}

// ———————————————————————————————————————————————
// ———————————————————————————————————————————————
// ———————————————————————————————————————————————



// ———————————————————————————————————————————————
// ——————————————————— page indexing —————————————
// ———————————————————————————————————————————————

// —————————————————————————————————————————————————————————
// ——————————————————————————————————————————————— MAIN GAME
void GameWin() {
  gameStatus=GameState::WON;
  Serial.println("HOORAY! :D");
  Stop_Timer();
  pageNum=Page::RESULTS_HUD;
}
void GameLose() {
  gameStatus==GameState::LOST;
  Serial.println("sadge :(");
  pageNum=Page::RESULTS_HUD;
}

bool Confirmation(){
  if (DeathmatchMode()) {
    cGuesses++;
    if (cGuesses<=cGuessesNeeded) return false;
    return true;
  } else if (SpeedMode()) {
    
    return true;
  } else {

    return true;
  }
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
  return false;
}

void PlayGame() {
<<<<<<< HEAD
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
=======
  if (Cnfrm()) {
    pageNum=Page::GAME_HUD;
    if (Idle()) {
      gameStatus=GameState::PLAYING;
      int diffIndex=static_cast<int>(difficulty);

      // ———————— DEATHMATCH ———————— //
      if (DeathmatchMode()) {
        int guessStash[]={20,18,14,10}; // guesses stash by difficulty
        int correctGoal[]={5,6,7,8};    // correct guesses goal by difficulty
        guessesLeft=guessStash[diffIndex];
        cGuessesNeeded=correctGoal[diffIndex];
        cGuesses=0;

        int minsStash[]={10,7,6,5};  // minutes by difficulty
        int secsStash[]={0,30,15,0}; // seconds by difficulty
        Update_Timer(minsStash[diffIndex],secsStash[diffIndex]);

      // ———————— SPEED ———————— //
      } else if (SpeedMode()) {
        int minsStash[]={10,5,3,2};
        Update_Timer(minsStash[diffIndex],0);

      // ———————— WORDLE ———————— //
      } else {
        guessesLeft=6;
        int diffWordLen[]={5,6,6,7};
        wordLength=diffWordLen[diffIndex];
      }
      RandomWord(secretWord,sizeof(secretWord),wordLength);
      if (DEBUG_ON()) {
        // Serial.print("The secret word is.. "); Serial.println(secretWord);
        Serial.println(" ——————————— ——————————— ——————————— ");
        Serial.println(" ——————————— !! BEGIN !! ——————————— ");
        Serial.println(" ——————————— ——————————— ——————————— ");
      }
      ClrLetters();
      ResetPos();
      if (!WordleMode()) {
        Play_Timer();
      }
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
    }
  }
}

void update_Status() {
<<<<<<< HEAD
  if (CheckPage(Page::RESULTS)) return;
  if (!CheckStatus(GameState::PLAYING)) return;
  if (!CheckMode(Gamemode::REGULAR)) {
=======
  if (pageNum==Page::RESULTS_HUD) return;
  if (!IsPlaying()) return;
  if (!WordleMode()) {
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
    if (NoTime()) {
      GameLose();
    }
  } else {
<<<<<<< HEAD
    if (wrong_guesses>=6) {
=======
    if (guesses>=6) {
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
      GameLose();
    }
  }
}

void update_GameHud() {
  SeekLetter();
  // DEBUG_CharSelect();
<<<<<<< HEAD
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
=======

  if (Cnfrm()) {
    bool valid=WordValid(arrangement);
    if (DEBUG_ON()) {
      Serial.print("Is a valid word? ");
      Serial.println(valid);
    }
    if (valid) {
      bool correct=CorrectWord(arrangement,secretWord);
      DEBUG_Indicators();
      guesses++;
      if (correct) {
        if (Confirmation()) {
          GameWin();
        }
      }
    }
  }
}
// ——————————————————————————————————————————————— MAIN GAME
// —————————————————————————————————————————————————————————

// ———————————————————————————————————————————————————————————————
// ——————————————————————————————————————————————— DIFFICULTY PAGE
void goto_DiffPage() {
  pageNum=Page::DIFF_MENU;
  // Serial.println("Select difficulty level! <- ->");
}
void update_DiffPage() {
  int numDiffs=static_cast<int>(Difficulty::numDiffs);
  Scrolling(&charPos,numDiffs,-UpDwn());
  // if (LftRgt()==0) { DEBUG_Difficulty(); }
  if (LftRgt()==-1) goto_GamemodePage();
  if (Cnfrm()) {
    difficulty=static_cast<Difficulty>(charPos);
    ResetPos();
    PlayGame();
  }
}
// ——————————————————————————————————————————————— DIFFICULTY PAGE

// —————————————————————————————————————————————————————————————
// ——————————————————————————————————————————————— GAMEMODE PAGE
void goto_GamemodePage() {
  pageNum=Page::GAMEMODE_MENU;
  // Serial.println("Choose your gamemode! <- ->");
}
void update_GamemodePage() {
  int numModes=static_cast<int>(Gamemode::numGamemodes);
  Scrolling(&charPos,numModes,-UpDwn());
  // if (LftRgt()==0) { DEBUG_Gamemode(); }
  if (LftRgt()==-1) goto_MainMenu();
  if (Cnfrm()) {
    gameMode=static_cast<Gamemode>(charPos);
    ResetPos();
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
    switch(gameMode) {
      case Gamemode::REGULAR: break;
      case Gamemode::SPEED: break;
      case Gamemode::DEATHMATCH: break;
    }
<<<<<<< HEAD
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
=======
    goto_DiffPage();
  }
}
// ——————————————————————————————————————————————— GAMEMODE PAGE
// —————————————————————————————————————————————————————————————

// —————————————————————————————————————————————————————————
// ——————————————————————————————————————————————— MAIN MENU
void goto_MainMenu() {
  gameStatus=GameState::NONE;
  gameMode=Gamemode::REGULAR;
  pageNum=Page::MAIN_MENU;
  guesses=0;
}
void update_MainMenu() {
  Scrolling(&charPos,4,-UpDwn());
  if (Cnfrm()&&(charPos==0)) {
    goto_GamemodePage();
  }
}
// ——————————————————————————————————————————————— MAIN MENU
// —————————————————————————————————————————————————————————

>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
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
<<<<<<< HEAD
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
=======
  // DEBUG_PageNum();
  switch (pageNum) {
    case Page::MAIN_MENU: update_MainMenu(); break;
    case Page::GAMEMODE_MENU: update_GamemodePage(); break;
    case Page::DIFF_MENU: update_DiffPage(); break;
    case Page::GAME_HUD: update_GameHud(); break;
    case Page::RESULTS_HUD: print_Results(); break;
    default: break;
  }
}

// ———————————————————————————————————————————————
// ———————————————————————————————————————————————
// ———————————————————————————————————————————————
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
