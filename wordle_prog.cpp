#include "wordle_prog.h"

const char* const wordGuesses[] PROGMEM={}; // current words already guessed.

// ——————————————————— program flags ——————
Page pageNum=Page::MAIN_MENU;               // current page
GameState gameStatus=GameState::NONE;       // current result
Gamemode gameMode=Gamemode::REGULAR;
Difficulty difficulty=Difficulty::EASY;
// ————————————————————————————————————————

// ——————————————————— number positioning —
int charPos=0;             // pos of current letter of guessing word (and main cursor of selection)
int letterPos=0;           // pos of alphabet selection

// ——————— REGULAR ———————————————————————————
size_t wordLength=5;         // current secret word length
char secretWord[10];
char arrangement[6]={};      // current letter arrangement
LetterState indicator[6]={}; // current letter indicators
size_t guesses=0;

// ——————— SPEED —————————————————————
size_t points=0;
size_t wGuesses=0; // wrongGuesses;

// ——————— DEATHMATCH —————————————————————
size_t guessesLeft=0;
size_t cGuessesNeeded=0;
size_t cGuesses=0;

// Chooses a random word from the word bank depending on the # of letters.
void RandomWord(char *buffer,size_t bufferSize,int wordSize) {
  const size_t totalWords=WC5;
  size_t randomIndex=random(totalWords);
  strncpy_P(buffer,words5[randomIndex],bufferSize-1);
  buffer[bufferSize-1]='\0';
}

// Checks to see if the word exists from the word bank.
bool WordValid(const char guess[],const size_t wordLength) {
  bool isValid=false;
  uint16_t numWords=WC5;
  for (size_t i=0;i<numWords;i++) {
    char currentWord[wordLength+1];
    strncpy_P(currentWord,words5[i],sizeof(currentWord)-1);
    currentWord[sizeof(currentWord)-1]='\0';
    size_t numLetters=strlen(currentWord)+1;
    if (numLetters!=(wordLength+1)) continue;
    isValid=((strcmp(guess,currentWord))==0);
    if (isValid) break;
  }
  return isValid;
}

// Checks for the matching letters and sequence with the secret word.
// Also updates the indicators to help signal letter correctness.
bool CorrectWord(const char guess[],const char secret[]) {
  const size_t gLen=strlen(guess);
  const size_t sLen=strlen(secret);
  if (gLen!=sLen) return false;
  size_t lettersCorrect=0;
  for (size_t i=0;i<5;i++) {
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

bool Easy() { return difficulty==Difficulty::EASY; }
bool Intermediate() { return difficulty==Difficulty::INTERMEDIATE; }
bool Expert() { return difficulty==Difficulty::EXPERT; }
bool Nightmare() { return difficulty==Difficulty::NIGHTMARE; }

// ————————————————————————————————————————————————
// ——————————————————— button seeking —————————————
// ————————————————————————————————————————————————

void SeekLetter() {
  if (!NoResp()) {
    int alphaRange=(MAX_ALPHABET-MIN_ALPHABET);
    int ud=UpDwn();
    int lr=LftRgt();
    size_t guessSize=LENG(arrangement);
    // DEBUG_Direction();
    if (ud!=0) { // USER presses UP or DOWN.
      int index=(letterPos+(ud*scrollSpeed)); // adds -1 or +1 to current letter pos
      letterPos=(index<=0)?(alphaRange+1):(index>alphaRange+1)?1:index; // adds -1 or +1, from 1 to 26.
      arrangement[charPos]=(letterPos-1)+MIN_ALPHABET; // selects the char from the letterPos.
    } else if (lr!=0) { // USER presses LEFT or RIGHT.
      int offset=(charPos+lr);
      int nextIndex=((offset<0)?(5-1):offset)%5; // safeguard index from 0 to # of letters in current word.
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
  return false;
}

void PlayGame() {
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
        int diffWordLen[]={5,6,7,8};
        wordLength=diffWordLen[diffIndex];
      }
      RandomWord(secretWord,sizeof(secretWord),5);
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
    }
  }
}
void update_GameHud() {
  SeekLetter();
  DEBUG_CharSelect();

  if (Cnfrm()) {
    bool valid=WordValid(arrangement,5);
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

  if (NoTime() && IsPlaying() && Won()) {
    GameLose();
  }
}
// ——————————————————————————————————————————————— MAIN GAME
// —————————————————————————————————————————————————————————

// ———————————————————————————————————————————————————————————————
// ——————————————————————————————————————————————— DIFFICULTY PAGE
void goto_DiffPage() {
  pageNum=Page::DIFF_MENU;
  Serial.println("Select difficulty level! <- ->");
}
void update_DiffPage() {
  int numDiffs=static_cast<int>(Difficulty::numDiffs);
  Scrolling(&charPos,numDiffs,-UpDwn());
  if (LftRgt()==0) { DEBUG_Difficulty(); }
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
  Serial.println("Choose your gamemode! <- ->");
}
void update_GamemodePage() {
  int numModes=static_cast<int>(Gamemode::numGamemodes);
  Scrolling(&charPos,numModes,-UpDwn());
  if (LftRgt()==0) { DEBUG_Gamemode(); }
  if (Cnfrm()) {
    gameMode=static_cast<Gamemode>(charPos);
    ResetPos();
    switch(gameMode) {
      case Gamemode::REGULAR: break;
      case Gamemode::SPEED: break;
      case Gamemode::DEATHMATCH: break;
    }
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
  if (Cnfrm()) {
    goto_GamemodePage();
  }
}
// ——————————————————————————————————————————————— MAIN MENU
// —————————————————————————————————————————————————————————

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
  DEBUG_PageNum();
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