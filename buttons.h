#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <Arduino.h>
#include <assert.h>

// ——————————————————— button object & enum flags —
enum class ButtonAction { NONE,UP,DWN,LFT,RGT,CNFRM };
enum class ButtonToggle { HOLD,CLICK };
struct Button {
  const byte PIN;
  const ButtonAction ACTION;
  const ButtonToggle TOGGLE;
  unsigned long LAST_DB_TIME;
  int LAST;
  bool REPEATING;
  int C;
};
// ————————————————————————————————————————————————

// ——————————————————— settings ———————————
#define MIN_DIAL_SPEED 1
#define MAX_DIAL_SPEED 3
#define DIAL_STEPPING 10

#define DEBOUNCE_DELAY 25
#define HOLD_DELAY 350
#define HOLD_REPEAT_RATE 100
// ————————————————————————————————————————

// ——————————————————— number positioning —
extern Button buttons[];
extern int scrollSpeed;
extern byte numButtons;
extern ButtonAction currentAction;
// ————————————————————————————————————————

// ——————————————————— functions ——————————
// bool flags
bool Cnfrm();
bool Up();
bool Dwn();
bool Lft();
bool Rgt();
bool NoResp();

// toggle flags
int UpDwn();
int LftRgt();

// buffer control
void Scrolling(int *cursorPos,int maximum,int dir);

// utility functions
void Init_Buttons();
ButtonAction GetButtonPress();
// ————————————————————————————————————————

#endif