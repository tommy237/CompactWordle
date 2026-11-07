// remember to download U8g2 from the Arduino Library Manager first!
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
  AutomaticResponse();
}

void AutomaticResponse() {
  if (!OnPopup()) {
    UpdateTime(false);
    if (!NoResp()) { update_Screen();}
    update_Status();
  }
  OnWait();
}