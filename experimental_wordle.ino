#include <Arduino.h>
#include "wordle_prog.h"

void setup() {
  Serial.begin(9600);
  Init_LCD();
  Init_Buttons();
  randomSeed(analogRead(0));
  Animate_Intro();
}

void loop() {
  screen.firstPage();
  do {
    Navigate_LCD();
  } while (screen.nextPage());
  currentAction=GetButtonPress();
  Automatic_Response();
}

void Automatic_Response() {
  Update_Time();
  if (!NoResp()) {
    // DEBUG_ButtonPress();
    update_Screen();
  }
}
