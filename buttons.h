#ifndef __BUTTONS_H__
#define __BUTTONS_H__

// ——————————————————— headers ———————————— //
#include <Arduino.h>
#include <assert.h>
// ——————————————————— headers ———————————— //



// ——————————————————— enum flags ————————— //
enum class ButtonAction { NONE,UP,DWN,LFT,RGT,CNFRM };
enum class ButtonToggle { HOLD,CLICK };
// ——————————————————— enum flags ————————— //



// ——————————————————— structures ———— //
struct Button {
  const byte PIN;
  const ButtonAction ACTION;
  const ButtonToggle TOGGLE;
  unsigned long LAST_DB_TIME;
  int LAST;
  bool REPEATING;
  int C;
};
// ——————————————————— structures ———— //



// ——————————————————— macros ————————————— //
#define MIN_DIAL_SPEED 1
#define MAX_DIAL_SPEED 3
#define DIAL_STEPPING 10

#define DEBOUNCE_DELAY 25
#define HOLD_DELAY 350
#define HOLD_REPEAT_RATE 100
// ——————————————————— macros ————————————— //



// ——————————————————— number positioning — //
extern Button buttons[];
extern byte numButtons;
// ——————————————————— number positioning — //



// —————————————— public functions ————————— //
bool OnLocked();
bool Cnfrm();
bool Up();
bool Dwn();
bool Lft();
bool Rgt();
bool NoResp();

uint8_t GetScrollSpeed();
int8_t UpDwn();
int8_t LftRgt();

void LockButtons(bool toggle);
void Scrolling(int8_t *cursorPos,int maximum,int dir);
void Init_Buttons();
ButtonAction GetButtonPress();
void OnResponse();
// —————————————— public functions ————————— //

#endif