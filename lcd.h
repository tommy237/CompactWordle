#ifndef __LCD_H__
#define __LCD_H__

<<<<<<< HEAD
// ——————————————————— headers ————————————
#include "U8g2lib.h"
#include "wordle_prog.h"
#include "popups.h"
// ————————————————————————————————————————



// ——————————————————— enum flags —————————
enum class LCDColor { BLACK,WHITE,XOR };
enum class TextPos { TOP_LEFT,TOP_RIGHT,BTM_LEFT,BTM_RIGHT };
=======
#include "U8g2lib.h"
#include "wordle_prog.h"

// ——————————————————— enum flags —————————
enum class LCDColor { BLACK,WHITE,XOR };
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ————————————————————————————————————————



// ——————————————————— pinout —————————————
#define RS_PIN 51 // first pin to LCD
#define RW_PIN 53 // second pin to LCD
#define E_PIN  52 // third pin to LCD
// ————————————————————————————————————————



// ——————————————————— settings ———————————
<<<<<<< HEAD
#define LCD_WIDTH 128       // max LCD width  (in pixels)
#define LCD_HEIGHT 64       // max LCD height (in pixels)
#define SCROLL_SPACING 2    // vertical spacing between selected item
#define LCD_CHAR_SPACING 1  // horizontal spacing between characters in game
#define NUM_GUIDE_PAGES 20
=======
#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define SCROLL_SPACING 2
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ————————————————————————————————————————



// ——————————————————— global vars ————————
extern U8G2_ST7920_128X64_1_SW_SPI screen;
<<<<<<< HEAD
extern const char* MENU_BUTTONS[];
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
// ————————————————————————————————————————



// ——————————————————— functions ——————————
<<<<<<< HEAD
bool OnPopup();
void SetDisplayFlag(bool toggle);
=======
>>>>>>> a40aaa0efd75482f02dc602d369ae940ad17631c
void Animate_Intro();
void Init_LCD();
void Navigate_LCD();
// ————————————————————————————————————————

#endif