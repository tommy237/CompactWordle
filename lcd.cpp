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
  screen.setDrawColor(static_cast<int>(pte)+1);
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
  uint8_t dWidth=screen.getDisplayWidth();
  uint8_t dHeight=screen.getDisplayHeight();
  int bHeight=8;
  for (int c=0;c<maximum;c++) {
    char* selected=selects[c];
    bool matchIndex=(c==charPos);
    int diff=(c-charPos);
    int pos=(7*diff);
    if (abs(diff)>2) continue;
    pos=(!matchIndex)?((diff>0)?pos+1:pos-2):pos;
    CenterText(0,pos,selected);
    if (matchIndex) {
      int bWidth=screen.getStrWidth(selected)+4;
      int arrowSpacing=1;
      uint8_t backSide=MidPt(dWidth,0)-(bWidth/2)-10-arrowSpacing;
      uint8_t pt1[2]={backSide,MidPt(dHeight,0)-(bHeight/2)};
      uint8_t pt2[2]={backSide,MidPt(dHeight,0)+(bHeight/2)};
      uint8_t pt3[2]={backSide+7,MidPt(dHeight,0)};
      MakeArrow(pt1,pt2,pt3);
      // box dimensions
      SetColorMode(LCDColor::XOR);
      screen.drawBox(
        MidPt(LCD_WIDTH,bWidth),
        MidPt(LCD_HEIGHT,bHeight),
        bWidth,bHeight);
      SetColorMode(LCDColor::NORMAL);
    }
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
  screen.drawStr(33,10,"MAIN MENU");
  
}

void LCD_Gamemode() {
  const char* list[]={"REGULAR","SPEED","DEATHMATCH"};
  Navigate_Select(static_cast<int>(Gamemode::numGamemodes),list);
}

void LCD_Difficulty() {
  const char* list[]={"EASY","MODERATE","EXPERT","HARDCORE"};
  Navigate_Select(static_cast<int>(Difficulty::numDiffs),list);
}

void LCD_Game() {
  screen.setFont(u8g2_font_helvB08_tr);
  const int numSpaces=wordLength-1;
  const int spacing=5; // in pixels
  char dummy[wordLength+1];
  memset(dummy,'A',sizeof(dummy));
  dummy[wordLength]='\0';
  const int tWidth=(wordLength*7)+(numSpaces*spacing);
  const int cX=MidPt(screen.getDisplayWidth(),tWidth);
  const int cY=MidPt(screen.getDisplayHeight(),screen.getMaxCharHeight());
  int currentX=cX;
  for (int c=0;c<wordLength;c++) {
    char current=arrangement[c];
    bool matchIndex=(c==charPos);
    bool noExist=(current=='\0');
    current=(noExist?'_':current);
    int bWidth=screen.getStrWidth(noExist?'A':current)+1;
    screen.drawGlyph(currentX,cY,current);
    if (matchIndex) {
      int bHeight=8;
      SetColorMode(LCDColor::XOR);
      screen.drawBox(
        currentX-(bWidth/2)-1,
        cY-(bHeight/2)-1,
        bWidth+1,
        (bHeight+1));
      SetColorMode(LCDColor::NORMAL);
    }
    currentX+=(bWidth+spacing);
  }
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