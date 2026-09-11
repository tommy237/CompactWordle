#ifndef __LCD_H__
#define __LCD_H__

#include "U8g2lib.h"
#include "wordle_prog.h"

// ——————————————————— enum flags —————————
enum class LCDColor { BLACK,WHITE,XOR };
// ————————————————————————————————————————



// ——————————————————— pinout —————————————
#define RS_PIN 51 // first pin to LCD
#define RW_PIN 53 // second pin to LCD
#define E_PIN  52 // third pin to LCD
// ————————————————————————————————————————



// ——————————————————— settings ———————————
#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define SCROLL_SPACING 2
// ————————————————————————————————————————



// ——————————————————— global vars ————————
extern U8G2_ST7920_128X64_1_SW_SPI screen;
// ————————————————————————————————————————



// ——————————————————— functions ——————————
void Animate_Intro();
void Init_LCD();
void Navigate_LCD();
// ————————————————————————————————————————

#endif