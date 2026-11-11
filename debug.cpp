#include "debug.h"

Debug debugMode = Debug::ON;

bool DEBUG_ON() { return debugMode == Debug::ON; }
bool DEBUG_OFF() { return debugMode == Debug::OFF; }

void DEBUG_PageNum() {
  if (DEBUG_ON()) {
    Serial.print("PageNum: ");
    Serial.println(static_cast<int>(GetPageNum()));
  }
}

void DEBUG_ButtonPress() {
  if (DEBUG_ON()) {
    Serial.print("ButtonPress: ");
    Serial.println(static_cast<int>(GetAction()));
  }
}

void DEBUG_Indicators() {
  if (DEBUG_ON()) {
    Serial.print(" / Correctness : ");
    for (uint8_t c = 0; c < GetWordLength(); c++) {
      Serial.print(" ");
      LetterState current=GetLetterStatus(c);
      switch (current) {
        case LetterState::INCORRECT: Serial.print("0"); break;
        case   LetterState::PARTIAL: Serial.print("1"); break;
        case     LetterState::MATCH: Serial.print("2"); break;
        default: Serial.print("0"); break;
      }
    }
    Serial.println(" /");
  }
}

void DEBUG_CharSelect() {
  if (DEBUG_ON() && !NoResp()) {  
    Serial.print("LetterPos: ");
    Serial.print(GetVerticalPos());
    Serial.print(" | CharPos: ");
    Serial.print(GetHorizontalPos());
    Serial.print(" | Arrangement: ");
    for (uint8_t c = 0; c < 5; c++) {
      char current=arrangement[c];
      switch (current) {
        case '\0': Serial.print("_"); break;
        default: Serial.print(current); break;
      }
    }
    Serial.println("");
  }
}


void DEBUG_Direction() {
  if (DEBUG_ON()) {
    Serial.print("Vertical direction: ");
    Serial.print(UpDwn());
    Serial.print(" | Horizontal direction: ");
    Serial.println(LftRgt());
  }
}

void DEBUG_ValidWord() {
  if (DEBUG_ON()) {
    Serial.print("Is a valid word? ");
    Serial.println(GetLastWordValid());
  }
}

void DEBUG_DisplayWord(char* word) {
  if (DEBUG_ON()) {
    Serial.print("The word is.. ");
    Serial.println(word);
  }
}

void DEBUG_Gamemode() {
  if (DEBUG_ON()) {
    Serial.print("Gamemode: ");
    switch(static_cast<Gamemode>(GetHorizontalPos())) {
      case Gamemode::REGULAR: Serial.println("[a]    REGULAR"); break;
      case Gamemode::SPEED: Serial.println("[b]      SPEED"); break;
      case Gamemode::DEATHMATCH: Serial.println("[c] DEATHMATCH"); break;
    }
  }
}

void DEBUG_Difficulty() {
  if (DEBUG_ON()) {
    Serial.print("Difficulty: ");
    switch(static_cast<Difficulty>(GetHorizontalPos())) {
      case Difficulty::NORMAL: Serial.println("*    (NORMAL)"); break;
      case Difficulty::MODERATE: Serial.println("**   (MODERATE)"); break;
      case Difficulty::EXPERT: Serial.println("***  (EXPERT)"); break;
      case Difficulty::HARDCORE: Serial.println("**** (HARDCORE)"); break;
    }
  }
}

void DEBUG_Time() {
  if (DEBUG_ON()) {
    const int m = GetMinutes(),
              s = GetSeconds();
    Serial.print("TIME: ");
    if (m > 0) {
      Serial.print(m);
      Serial.print("m ");
    }
    Serial.print(s);
    Serial.println("s");
  }
}

void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp)
{
  Serial.println("\n————————————————— ASSERTION FAILED —————————————————");
  Serial.print("File: "); Serial.println(__file);
  Serial.print("Line: "); Serial.println(__lineno);
  Serial.print("Function: "); Serial.println(__func);
  Serial.print("Expression: "); Serial.println(__sexp);
  Serial.flush();
  
  // Halts the microcontroller. Use an infinite loop.
  while (true) {
    // Optional: blink an LED here to give a physical warning light
  }
}