#ifndef __BUTTONS_H__
#define __BUTTONS_H__

<<<<<<< HEAD
// ——————————————————— headers ————————————
#include <Arduino.h>
#include <assert.h>
// ————————————————————————————————————————


=======
#include <Arduino.h>
#include <assert.h>
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c

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

<<<<<<< HEAD


=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ——————————————————— settings ———————————
#define MIN_DIAL_SPEED 1
#define MAX_DIAL_SPEED 3
#define DIAL_STEPPING 10

#define DEBOUNCE_DELAY 25
#define HOLD_DELAY 350
#define HOLD_REPEAT_RATE 100
// ————————————————————————————————————————

<<<<<<< HEAD


=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ——————————————————— number positioning —
extern Button buttons[];
extern int scrollSpeed;
extern byte numButtons;
extern ButtonAction currentAction;
// ————————————————————————————————————————

<<<<<<< HEAD


// ——————————————————— functions ——————————
bool OnLocked();

=======
// ——————————————————— functions ——————————
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
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
<<<<<<< HEAD
void LockButtons(bool toggle);
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ————————————————————————————————————————

#endif