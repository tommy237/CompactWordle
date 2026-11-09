// ——————————————————— headers ———————————— //
#include "buttons.h"
// ——————————————————— headers ———————————— //



// —————— data storage ————————————————————— //
Button buttons[]={
  {23,ButtonAction::UP,   ButtonToggle::HOLD, 0,HIGH,false},
  {24,ButtonAction::DWN,  ButtonToggle::HOLD, 0,HIGH,false},
  {22,ButtonAction::LFT,  ButtonToggle::HOLD, 0,HIGH,false},
  {25,ButtonAction::RGT,  ButtonToggle::HOLD, 0,HIGH,false},
  {27,ButtonAction::CNFRM,ButtonToggle::CLICK,0,HIGH,false}
};
uint8_t scroll_speed=MIN_DIAL_SPEED; // speed of char scrolling when holding.
byte numButtons=sizeof(buttons)/sizeof(buttons[0]);
ButtonAction currentAction;
bool buttonLock=false;
// —————— data storage ————————————————————— //



// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

bool OnLocked() { return buttonLock; }
bool Cnfrm() { return currentAction==ButtonAction::CNFRM; }
bool Up() { return currentAction==ButtonAction::UP; }
bool Dwn() { return currentAction==ButtonAction::DWN; }
bool Lft() { return currentAction==ButtonAction::LFT; }
bool Rgt() { return currentAction==ButtonAction::RGT; }
bool NoResp() { return currentAction==ButtonAction::NONE; }

uint8_t GetScrollSpeed() { return scroll_speed; }
int8_t UpDwn() { return Dwn()?-1:Up()?1:0; } // up 1 or down 1
int8_t LftRgt() { return Lft()?-1:Rgt()?1:0; } // left 1 or right 1


// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |—————————————————————————————————————————————| //
// |————| DISPLAY FUNCTIONS |————————————————————| //

void LockButtons(bool toggle) { buttonLock=toggle; }

void Scrolling(int8_t *cursorPos,int maximum,int dir) {
  assert(abs(dir)<=1);
  assert(maximum>0);
  int nextPos=(*cursorPos)+dir;
  *cursorPos=(nextPos%maximum+maximum)%maximum; // standard formula for a circular buffer
}

void Init_Buttons() {
  for (byte e=0;e<numButtons;e++) {
    Button *btn=&buttons[e];
    pinMode(btn->PIN,INPUT_PULLUP);
  }
}

ButtonAction GetButtonPress() {
  const unsigned long ms=millis();
  ButtonAction triggered=ButtonAction::NONE;
  if (buttonLock) return triggered;
  for (byte e=0;e<numButtons;e++) {
    Button *btn=&buttons[e];
    const int state=digitalRead(btn->PIN);
    const unsigned long diff=(ms-btn->LAST_DB_TIME);
    const bool above_deb=(diff>DEBOUNCE_DELAY),
               pressed=(btn->LAST==HIGH && state==LOW),
               holding=(btn->LAST==LOW && state==LOW),
               released=(state==HIGH && above_deb);
    if (pressed) {
      if (above_deb) {
        triggered=btn->ACTION;
        btn->LAST_DB_TIME=ms;
        btn->LAST=LOW;
        btn->REPEATING=false;
      }
    } else if (holding) {
      if (btn->TOGGLE==ButtonToggle::HOLD) {
        if (!(btn->REPEATING)) {
          if (diff>=HOLD_DELAY) {
            triggered=btn->ACTION;
            btn->LAST_DB_TIME=ms;
            btn->REPEATING=true;
          }
        } else {
          if (diff>=HOLD_REPEAT_RATE) {
            triggered=btn->ACTION;
            btn->LAST_DB_TIME=ms;
            btn->C++;
            float t=constrain(btn->C,0,DIAL_STEPPING);
            float f_x=pow(t/DIAL_STEPPING,3);
            scroll_speed=map(f_x*1E2,0,1E2,
              MIN_DIAL_SPEED,
              MAX_DIAL_SPEED);
          }
        }
      }
    } else if (released) {
      if (btn->REPEATING) {
        btn->C=0;
        scroll_speed=MIN_DIAL_SPEED;
      }
      btn->REPEATING=false;
      btn->LAST=HIGH;
    }
  }
  return triggered;
}

void OnResponse() {
  currentAction=GetButtonPress();
}

// |————| DISPLAY FUNCTIONS |————————————————————| //
// |—————————————————————————————————————————————| //