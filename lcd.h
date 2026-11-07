#ifndef __LCD_H__
#define __LCD_H__

// ——————————————————— headers ————————————
#include "U8g2lib.h"
#include "wordle_prog.h"
#include "popups.h"
// ————————————————————————————————————————



// ——————————————————— enum flags —————————
enum class LCDColor { BLACK,WHITE,XOR };
enum class TextPos { TOP_LEFT,TOP_RIGHT,BTM_LEFT,BTM_RIGHT };
// ————————————————————————————————————————



// ——————————————————— pinout —————————————
#define RS_PIN 51 // first pin to LCD
#define RW_PIN 53 // second pin to LCD
#define E_PIN  52 // third pin to LCD
// ————————————————————————————————————————



// ——————————————————— settings ———————————
#define LCD_WIDTH 128       // max LCD width  (in pixels)
#define LCD_HEIGHT 64       // max LCD height (in pixels)
#define SCROLL_SPACING 2    // vertical spacing between selected item
#define LCD_CHAR_SPACING 1  // horizontal spacing between characters in game
#define NUM_GUIDE_PAGES 20
// ————————————————————————————————————————



// ——————————————————— global vars ————————
extern U8G2_ST7920_128X64_1_SW_SPI screen;
extern const char* MENU_BUTTONS[];
// ————————————————————————————————————————



// ——————————————————— functions ——————————
bool OnPopup();
void SetDisplayFlag(bool toggle);
void Animate_Intro();
void Init_LCD();
void Navigate_LCD();
// ————————————————————————————————————————

#endif