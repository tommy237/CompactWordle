#ifndef __BUTTONS_H__
#define __BUTTONS_H__

// —————— headers ————————————————————————— //
#include <Arduino.h>
#include <assert.h>
// —————— headers ————————————————————————— //



// —————— enum flags —————————————————————— //
enum class ButtonAction { NONE,UP,DWN,LFT,RGT,CNFRM };
enum class ButtonToggle { HOLD,CLICK };
// —————— enum flags —————————————————————— //



// —————— structures ————————————————— //
struct Button {
  const byte pin;
  const ButtonAction action;
  const ButtonToggle toggle;
  unsigned long last_debounce;
  int last_state;
  bool is_repeating;
  int c;
};
// —————— structures ————————————————— //



// —————— macros —————————————————————————— //
#define MIN_DIAL_SPEED 1
#define MAX_DIAL_SPEED 3
#define DIAL_STEPPING 10

#define DEBOUNCE_DELAY 25
#define HOLD_DELAY 350
#define HOLD_REPEAT_RATE 100
// —————— macros —————————————————————————— //



// —————— global vars ————————————————————— //
extern Button buttons[];
extern uint8_t num_buttons;
// —————— global vars ————————————————————— //



// —————— public functions —————————————————————— //
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
void Scrolling(uint8_t *cursorPos, uint8_t max_i, int8_t dir);
void Init_Buttons();
ButtonAction GetAction();
ButtonAction GetButtonPress();
void OnResponse();
// —————— public functions —————————————————————— //

#endif