#include "debug.h"

Debug debugMode=Debug::ON;

bool DEBUG_ON() { return debugMode==Debug::ON; }
bool DEBUG_OFF() { return debugMode==Debug::OFF; }

void DEBUG_PageNum() {
  if (DEBUG_ON()) {
    Serial.print("PageNum: ");
    Serial.println(static_cast<int>(pageNum));
  }
}

void DEBUG_ButtonPress() {
  if (DEBUG_ON()) {
    Serial.print("ButtonPress: ");
    Serial.println(static_cast<int>(currentAction));
  }
}

void DEBUG_Indicators() {
  if (DEBUG_ON()) {
    Serial.print(" / Correctness : ");
    for (size_t c=0;c<5;c++) {
      Serial.print(" ");
      LetterState current=indicator[c];
      switch (current) {
        case LetterState::INCORRECT: Serial.print("0"); break;
        case LetterState::PARTIAL: Serial.print("1"); break;
        case LetterState::MATCH: Serial.print("2"); break;
        default: Serial.print("0"); break;
      }
    }
    Serial.println(" /");
  }
}

void DEBUG_CharSelect() {
  if (DEBUG_ON() && currentAction!=ButtonAction::CNFRM) {  
    Serial.print("LetterPos: "); Serial.print(letterPos);
    Serial.print(" | CharPos: "); Serial.print(charPos);
    Serial.print(" | Arrangement: ");
    for (int c=0;c<5;c++) {
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
    Serial.print("Vertical direction: "); Serial.print(UpDwn());
    Serial.print(" | Horizontal direction: "); Serial.println(LftRgt());
  }
}


void DEBUG_Gamemode() {
  if (DEBUG_ON()) {
    Serial.print("Gamemode: ");
    switch(static_cast<Gamemode>(charPos)) {
      case Gamemode::REGULAR: Serial.println("[a]    REGULAR"); break;
      case Gamemode::SPEED: Serial.println("[b]      SPEED"); break;
      case Gamemode::DEATHMATCH: Serial.println("[c] DEATHMATCH"); break;
    }
  }
}

void DEBUG_Difficulty() {
  if (DEBUG_ON()) {
    Serial.print("Difficulty: ");
    switch(static_cast<Difficulty>(charPos)) {
      case Difficulty::EASY: Serial.println("*    (EASY)"); break;
      case Difficulty::INTERMEDIATE: Serial.println("**   (INTERMEDIATE)"); break;
      case Difficulty::EXPERT: Serial.println("***  (EXPERT)"); break;
      case Difficulty::NIGHTMARE: Serial.println("**** (NIGHTMARE)"); break;
    }
  }
}

void DEBUG_Time() {
  if (DEBUG_ON()) {
    Serial.print("TIME: ");
    if (minutes>0) {
      Serial.print(minutes);
      Serial.print("m ");
    }
    Serial.print(seconds);
    Serial.println("s");
  }
}