#include "lcd.h"

U8G2_ST7920_128X64_1_SW_SPI screen(
  U8G2_R0,
  E_PIN,
  RW_PIN,
  RS_PIN
  // RST_PIN
);

void Init_LCD() {
  screen.begin();
}

// ———————————————————————————————————————————————
// ——————————————————— modified u8g2 —————————————
// ———————————————————————————————————————————————

void drawBorder(int x,int y,int w,int h,int t) {
  for (int th=0;th<t;th++) {
    screen.drawFrame(x+th,y+th,w-(th*2),h-(th*2));
  }
}

u8g2_uint_t MidPt(u8g2_uint_t pos1,u8g2_uint_t pos2) {
  return (pos1-pos2)/2;
}
void SetColorMode(LCDColor pte) {
  screen.setDrawColor(static_cast<int>(pte));
}
void CenterText(int x,int y,const char* str) {
  screen.setFontRefHeightText();
  screen.setFontPosTop();
  uint8_t dWidth=screen.getDisplayWidth();
  uint8_t dHeight=screen.getDisplayHeight();
  uint16_t sWidth=screen.getStrWidth(str);
  uint8_t sHeight=screen.getMaxCharHeight();
  int cX=(dWidth-sWidth)/2;
  int cY=(dHeight-sHeight)/2;
  screen.drawStr(cX+x,cY+(sHeight/3)+y,str);
}

void TopCenterText(const char* str) {
  screen.setFont(u8g2_font_tiny5_tr);
  const int cWidth=screen.getStrWidth(str);
  const int cHeight=(screen.getMaxCharHeight()/2)-3;
  int cX=MidPt(LCD_WIDTH,cWidth);
  screen.drawStr(cX,cHeight,str);
}

void TopText(const char* str) {
  screen.setFont(u8g2_font_tiny5_tr);
  const int cHeight=(screen.getMaxCharHeight()/2)-3;
  screen.drawStr(1,cHeight,str);
}

void TrackerText(const char* subttl,const char* str) {
  screen.setFont(u8g2_font_helvB08_tr);
  const int cHeight=screen.getMaxCharHeight();
  screen.drawStr(2,LCD_HEIGHT-(cHeight),str);

  screen.setFont(u8g2_font_tiny5_tr);
  const int nHeight=screen.getMaxCharHeight();
  screen.drawStr(2,LCD_HEIGHT-(cHeight+nHeight),subttl);
}

void MakeArrow(uint8_t pt1[2],uint8_t pt2[2],uint8_t pt3[2]) {
  screen.drawTriangle(
        pt1[0],pt1[1],
        pt2[0],pt2[1],
        pt3[0],pt3[1]);
}

// ———————————————————————————————————————————————
// ——————————————————— display functions —————————
// ———————————————————————————————————————————————

void Navigate_Select(int maximum,const char* selects[]) {
  screen.setFont(u8g2_font_tiny5duo_te);
  const int bHeight=8;
  for (int c=0;c<maximum;c++) {
    const char* selected=selects[c];
    const bool matchIndex=(c==charPos);
    const int diff=(c-charPos);
    int pos=(7*diff);
    if (abs(diff)>2) continue;
    pos=(!matchIndex)?((diff>0)?pos+1:pos-2):pos;
    CenterText(0,pos,selected);
    if (matchIndex) {
      const int bWidth=screen.getStrWidth(selected)+4;
      const int arrowSpacing=1;
      const uint8_t backSide=MidPt(LCD_WIDTH,0)-(bWidth/2)-10-arrowSpacing;
      screen.drawTriangle(
        backSide,MidPt(LCD_HEIGHT,0)-(bHeight/2),
        backSide,MidPt(LCD_HEIGHT,0)+(bHeight/2),
        backSide+7,MidPt(LCD_HEIGHT,0));
      SetColorMode(LCDColor::XOR);
      // box dimensions
      screen.drawBox(
        MidPt(LCD_WIDTH,bWidth),
        MidPt(LCD_HEIGHT,bHeight),
        bWidth,bHeight);
      SetColorMode(LCDColor::WHITE);
    }
  }
}

void Navigate_Character() {
  screen.setFont(u8g2_font_logisoso16_tr);
  const int cWidth=((screen.getMaxCharWidth()*2)/3); // char width
  const int cHeight=screen.getMaxCharHeight();       // char height
  const int numSpaces=(wordLength-1); // # spaces in between letters
  const int spacing=1; // how big the space between letters should be (in pixels)
  const int tWidth=(cWidth*wordLength)+(numSpaces*spacing); // total width
  const int cX=MidPt(LCD_WIDTH,tWidth); // center x string offset
  const int cY=MidPt(LCD_HEIGHT,cHeight); // center y string offset
  int curX=cX;
  for (int c=0;c<wordLength;c++) {
    char cur=arrangement[c];
    bool matchIndex=(c==charPos);
    bool noExist=(cur=='\0');
    cur=(noExist?' ':cur);
    screen.drawGlyph(curX,cY+2,cur);
    screen.drawGlyph(curX,cY+5,'_');
    if (matchIndex) {
      SetColorMode(LCDColor::XOR);
      screen.drawBox(curX,cY,cWidth,cHeight);
      SetColorMode(LCDColor::WHITE);
    }
    curX+=(cWidth+spacing);
  }
}

void Display_Difficulty() {
  const char* list[]={"WORDLE","SPEED","DEATHMATCH"};
  char selected[LCD_WIDTH/4];
  snprintf(selected,sizeof(selected),"< %s >",list[static_cast<int>(gameMode)]);
  TopCenterText(selected);
}

void Display_Timer() {
  if (TimeRunning()) {
    screen.setFont(u8g2_font_helvB08_tr);
    const int cHeight=screen.getMaxCharHeight();
    char timeArray[16];
    if (minutes>0) { sprintf(timeArray,"%dm %02ds",minutes,seconds); }
    else { sprintf(timeArray,"%ds",seconds); }
    TrackerText("time",timeArray);
  }
}

void Display_Progress() {
  screen.setFont(u8g2_font_tiny5_tr);
  const int cHeight=screen.getMaxCharHeight();
  int cWidth;
  char footer[16];
  if (SpeedMode()) {
    
  } else if (DeathmatchMode()) {

  } else {
    sprintf(footer,"%d/%d",guesses,6);
    TrackerText("guesses",footer);
  }
}

// ———————————————————————————————————————————————
// ——————————————————— page displaying ———————————
// ———————————————————————————————————————————————

void Animate_Intro() {

}

void LCD_Menu() {
  // border(0,0,128,64,2);
  screen.setFont(u8g2_font_helvB08_tr);
  const char* title="MENU";
  const int cHeight=screen.getMaxCharHeight();
  const int sWidth=screen.getStrWidth(title);
  const int cX=MidPt(LCD_WIDTH,sWidth);
  screen.drawStr(cX,0,title);
  const char* list[]={"PLAY","GUIDE","LEADERBOARD","CONFIG"};
  Navigate_Select(LENG(list)+1,list);
}

void LCD_Gamemode() {
  const char* list[]={"WORDLE","SPEED","DEATHMATCH"};
  Navigate_Select(static_cast<int>(Gamemode::numGamemodes),list);
  TopText("GAMEMODE");
}

void LCD_Difficulty() {
  const char* list[]={"EASY","MODERATE","EXPERT","HARDCORE"};
  Navigate_Select(static_cast<int>(Difficulty::numDiffs),list);
  TopText("DIFFICULTY");
}

void LCD_Game() {
  Navigate_Character();
  Display_Difficulty();
  Display_Timer();
  Display_Progress();
}

void LCD_Results() {

}

void Navigate_LCD() {
  switch(pageNum) {
    case Page::MAIN_MENU: LCD_Menu(); break;
    case Page::GAMEMODE_MENU: LCD_Gamemode(); break;
    case Page::DIFF_MENU: LCD_Difficulty(); break;
    case Page::GAME_HUD: LCD_Game(); break;
    case Page::RESULTS_HUD: LCD_Results(); break;
  }
}