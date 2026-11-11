// —————— headers ————————————————————————— //
#include "buttons.h"
// —————— headers ————————————————————————— //



// —————— data storage ————————————————————— //
Button buttons[] = {
  {23, ButtonAction::UP,    ButtonToggle::HOLD,  0, HIGH, false},
  {24, ButtonAction::DWN,   ButtonToggle::HOLD,  0, HIGH, false},
  {22, ButtonAction::LFT,   ButtonToggle::HOLD,  0, HIGH, false},
  {25, ButtonAction::RGT,   ButtonToggle::HOLD,  0, HIGH, false},
  {27, ButtonAction::CNFRM, ButtonToggle::CLICK, 0, HIGH, false}
};
uint8_t scroll_speed = MIN_DIAL_SPEED; // speed of char scrolling when holding.
uint8_t num_buttons = sizeof(buttons) / sizeof(buttons[0]);
ButtonAction curr_action;
bool buttonLock = false;
// —————— data storage ————————————————————— //



// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

bool OnLocked() { return buttonLock; }
ButtonAction GetAction() { return curr_action; };
bool Cnfrm()  { return GetAction() == ButtonAction::CNFRM; }
bool Up()     { return GetAction() == ButtonAction::UP; }
bool Dwn()    { return GetAction() == ButtonAction::DWN; }
bool Lft()    { return GetAction() == ButtonAction::LFT; }
bool Rgt()    { return GetAction() == ButtonAction::RGT; }
bool NoResp() { return GetAction() == ButtonAction::NONE; }

uint8_t GetScrollSpeed() { return scroll_speed; }
int8_t UpDwn()  { return Dwn() ? -1 : Up()  ? 1 : 0; } // up 1 or down 1
int8_t LftRgt() { return Lft() ? -1 : Rgt() ? 1 : 0; } // left 1 or right 1


// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |—————————————————————————————————————————————| //
// |————| DISPLAY FUNCTIONS |————————————————————| //

void LockButtons(bool toggle) { buttonLock = toggle; }

void Scrolling(uint8_t *cursorPos, uint8_t max_i, int8_t dir) {
  assert(abs(dir) <= 1);
  assert(max_i > 0);
  int8_t nextPos = (*cursorPos) + dir;
  *cursorPos = ((nextPos % max_i) + max_i) % max_i; // standard formula for a circular buffer
}

void Init_Buttons() {
  for (uint8_t e = 0; e < num_buttons; e++) {
    Button *btn = &buttons[e];
    pinMode(btn->pin, INPUT_PULLUP);
  }
}

ButtonAction GetButtonPress() {
  const unsigned long ms = millis();
  ButtonAction triggered = ButtonAction::NONE;
  if (buttonLock) return triggered;
  for (uint8_t e = 0; e < num_buttons; e++) {
    Button *btn = &buttons[e];
    const int state = digitalRead(btn->pin);
    const unsigned long diff = (ms - btn->last_debounce);
    const bool above_deb = (diff > DEBOUNCE_DELAY),
               pressed   = ((btn->last_state == HIGH) && (state == LOW)),
               holding   = ((btn->last_state == LOW) && (state == LOW)),
               released  = ((state == HIGH) && above_deb);
    if (pressed) {
      if (above_deb) {
        triggered = btn->action;
        btn->last_debounce = ms;
        btn->last_state = LOW;
        btn->is_repeating = false;
      }
    } else if (holding) {
      if (btn->toggle == ButtonToggle::HOLD) {
        if (!(btn->is_repeating)) {
          if (diff >= HOLD_DELAY) {
            triggered = btn->action;
            btn->last_debounce = ms;
            btn->is_repeating = true;
          }
        } else {
          if (diff >= HOLD_REPEAT_RATE) {
            triggered = btn->action;
            btn->last_debounce = ms;
            btn->c++;
            float t = constrain(btn->c, 0, DIAL_STEPPING);
            float f_x = pow(t / DIAL_STEPPING, 3);
            scroll_speed = map(f_x * 1E2, 0, 1E2,
              MIN_DIAL_SPEED,
              MAX_DIAL_SPEED);
          }
        }
      }
    } else if (released) {
      if (btn->is_repeating) {
        btn->c = 0;
        scroll_speed = MIN_DIAL_SPEED;
      }
      btn->is_repeating = false;
      btn->last_state = HIGH;
    }
  }
  return triggered;
}

void OnResponse() {
  curr_action = GetButtonPress();
}

// |————| DISPLAY FUNCTIONS |————————————————————| //
// |—————————————————————————————————————————————| //