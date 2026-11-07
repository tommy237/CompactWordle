<<<<<<< HEAD
// remember to download U8g2 from the Arduino Library Manager first!
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
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
<<<<<<< HEAD
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
=======
  Automatic_Response();
}

void Automatic_Response() {
  Update_Time();
  if (!NoResp()) {
    // DEBUG_ButtonPress();
    update_Screen();
  }
  update_Status();
}
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
