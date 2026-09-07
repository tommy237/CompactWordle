#include "buttons.h"

Button buttons[]={
  {23,ButtonAction::UP,   ButtonToggle::HOLD, 0,HIGH,false},
  {24,ButtonAction::DWN,  ButtonToggle::HOLD, 0,HIGH,false},
  {22,ButtonAction::LFT,  ButtonToggle::HOLD, 0,HIGH,false},
  {25,ButtonAction::RGT,  ButtonToggle::HOLD, 0,HIGH,false},
  {27,ButtonAction::CNFRM,ButtonToggle::CLICK,0,HIGH,false}
};
int scrollSpeed=MIN_DIAL_SPEED;
byte numButtons=sizeof(buttons)/sizeof(buttons[0]);
ButtonAction currentAction;

// ——————————————————— functions —
// returns if user pressed confirm button;
bool Cnfrm() { return currentAction==ButtonAction::CNFRM; }
bool Up() { return currentAction==ButtonAction::UP; }
bool Dwn() { return currentAction==ButtonAction::DWN; }
bool Lft() { return currentAction==ButtonAction::LFT; }
bool Rgt() { return currentAction==ButtonAction::RGT; }
bool NoResp() { return currentAction==ButtonAction::NONE; }

int UpDwn() { return Dwn()?-1:Up()?1:0; } // up 1 or down 1
int LftRgt() { return Lft()?-1:Rgt()?1:0; } // left 1 or right 1

void Scrolling(int *cursorPos,int maximum,int dir) {
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
  unsigned long ms=millis();
  ButtonAction triggered=ButtonAction::NONE;
  for (byte e=0;e<numButtons;e++) {
    Button *btn=&buttons[e];
    int state=digitalRead(btn->PIN);
    unsigned long msDiff=(ms-btn->LAST_DB_TIME);
    bool aboveThres=(msDiff>DEBOUNCE_DELAY);
    bool justPressed=(btn->LAST==HIGH && state==LOW);
    bool isHolding=(btn->LAST==LOW && state==LOW);
    bool justReleased=(state==HIGH && aboveThres);
    if (justPressed) {
      if (aboveThres) {
        triggered=btn->ACTION;
        btn->LAST_DB_TIME=ms;
        btn->LAST=LOW;
        btn->REPEATING=false;
      }
    } else if (isHolding) {
      if (btn->TOGGLE==ButtonToggle::HOLD) {
        if (!(btn->REPEATING)) {
          if (msDiff>=HOLD_DELAY) {
            triggered=btn->ACTION;
            btn->LAST_DB_TIME=ms;
            btn->REPEATING=true;
          }
        } else {
          if (msDiff>=HOLD_REPEAT_RATE) {
            triggered=btn->ACTION;
            btn->LAST_DB_TIME=ms;
            btn->C++;
            float t=constrain(btn->C,0,DIAL_STEPPING);
            float f_x=pow(t/DIAL_STEPPING,3);
            scrollSpeed=map(f_x*1E2,0,1E2,
              MIN_DIAL_SPEED,
              MAX_DIAL_SPEED);
          }
        }
      }
    } else if (justReleased) {
      if (btn->REPEATING) {
        btn->C=0;
        scrollSpeed=MIN_DIAL_SPEED;
      }
      btn->REPEATING=false;
      btn->LAST=HIGH;
    }
  }
  return triggered;
}

// ———————————————————————————————