#include "lcd.h"

struct SecretWord {char key[10];};

U8G2_ST7920_128X64_1_SW_SPI screen(U8G2_R0,E_PIN,RW_PIN,RS_PIN);
bool displayFlag=false;
const char* MENU_BUTTONS[]={"PLAY","GUIDE","STATS","RANKS","CONFIG"};

void Init_LCD() {
  screen.begin();
  screen.setFontRefHeightText();
  screen.setFontPosTop();
}

// ——————————————————————————————————————————————— //
// ——————————————————— modified u8g2 ————————————— //
// ——————————————————————————————————————————————— //

void SmallFont() { screen.setFont(u8g2_font_tiny5_tr); }
void BoldFont() { screen.setFont(u8g2_font_helvB08_tr); }
void SetColorMode(LCDColor pte) { screen.setDrawColor(static_cast<uint8_t>(pte)); }
void ShowNav(const uint8_t* xbmp) { screen.drawXBMP(2,(LCD_HEIGHT-7),(LCD_WIDTH-4),5,xbmp); }
bool Flashing(double i) { return (millis()%(long)(i*1E3))<(i*0.5E3); }
u8g2_uint_t MidPt(u8g2_uint_t pos1,u8g2_uint_t pos2) { return (pos1-pos2)/2; }

void CenterText(int x,int y,const char* str) {
  const uint8_t dW=screen.getDisplayWidth(),
                dH=screen.getDisplayHeight();
  const uint8_t sW=screen.getStrWidth(str),
                sH=screen.getMaxCharHeight();
  const uint8_t cX=(dW-sW)/2,cY=(dH-sH)/2;
  screen.drawStr(cX+x,cY+(sH/3)+y,str);
}

// Creates a text label with string centered on top of the display.
void TopCenterText(const char* str) {
  SmallFont();
  const uint8_t cW=screen.getStrWidth(str),
                cH=(screen.getMaxCharHeight()/2)-3;
  const uint8_t cX=MidPt(LCD_WIDTH,cW);
  screen.drawStr(cX,cH+1,str);
}

// Creates a text label with a subtitle and target value, often for in-game tracking
// or just to display the timer (jndicating loss or win for the game).
void TrackerText(const char* subttl,const char* str) {
  BoldFont();
  const uint8_t cH=screen.getMaxCharHeight();
  screen.drawStr(2,LCD_HEIGHT-(cH),str);
  SmallFont();
  const uint8_t nH=screen.getMaxCharHeight();
  screen.drawStr(2,LCD_HEIGHT-(cH+nH),subttl);
}

// Shorthand version of the u8g2.drawTriangle() with three
// int arrays as coordiante parameters.
void MakeArrow(uint8_t pt1[2],uint8_t pt2[2],uint8_t pt3[2]) {
  screen.drawTriangle(
    pt1[0],pt1[1], 
    pt2[0],pt2[1], 
    pt3[0],pt3[1]);
}

void BigText(uint8_t poX,uint8_t poY,const char* str) {
  BoldFont();
  // const int cH=screen.getMaxCharHeight();
  const uint8_t sW=screen.getStrWidth(str),
                cX=MidPt(LCD_WIDTH,sW);
  screen.drawStr(poX,poY,str);
}

void SmallText(uint8_t poX,uint8_t poY,const char* str) {
  SmallFont();
  const uint8_t cH=screen.getMaxCharHeight();
  screen.drawStr(poX,(poY-cH),str);
}

void StraightLine(const uint8_t h,const uint8_t padding) {
  screen.drawLine(padding,h,(LCD_WIDTH-(padding+1)),h);
}

void PositionSub(const char* str,TextPos pos) {
  const bool right=((pos==TextPos::TOP_RIGHT)||(pos==TextPos::BTM_RIGHT)),
             down=((pos==TextPos::BTM_LEFT)||(pos==TextPos::BTM_RIGHT));
  SmallFont();
  const uint8_t cW=screen.getStrWidth(str),
                cH=screen.getMaxCharHeight();
  const uint8_t x=right?((LCD_WIDTH)-(cW+2)):2,
                y=down?(LCD_HEIGHT-1):(cH+1);
  SetColorMode(LCDColor::BLACK);
  screen.drawBox((x-2),
    down?(LCD_HEIGHT-(cH+1)):0,
    (cW+4),(cH+1));
  SetColorMode(LCDColor::WHITE);
  SmallText(x,y,str);
}

// ——————————————————————————————————————————————— //
// ——————————————————— display functions ————————— //
// ——————————————————————————————————————————————— //

bool OnPopup() { return displayFlag; }

// Displays to LCD the current options of a
// row of buttons to press on confirm.
void NavigateSelect(size_t maximum,const char* selects[]) {
  screen.setFont(u8g2_font_tiny5duo_te);
  const uint8_t bH=8;
  for (size_t i=0;i<maximum;i++) {
    const char* c=selects[i];
    const bool match=(i==GetHorizontalPos()); // if index matches current option
    const int diff=(i-GetHorizontalPos());
    int pos=(7*diff);
    if (abs(diff)>2) continue; // omit further options until 2 positions closer to index
    pos=(!match)?((diff>0)?(pos+1):(pos-2)):pos; // spacing from matching index
    if (match) {
      const uint8_t arrowPadding=4,
                    arrowSpacing=8,
                    arrowThickness=4;
      const uint8_t bW=screen.getStrWidth(c)+arrowPadding;
      const uint8_t back=MidPt(LCD_WIDTH,0)-(bW/2)-arrowSpacing,
                    midH=MidPt(LCD_HEIGHT,0);
      screen.drawTriangle(back,               midH-(bH/2),  // top back
                          back,               midH+(bH/2),  // bottom back
                          back+arrowThickness,midH       ); // pointy tip
      // box dimensions
      screen.drawBox(MidPt(LCD_WIDTH,bW),MidPt(LCD_HEIGHT,bH-2),bW,bH-2);
      screen.drawBox(MidPt(LCD_WIDTH,bW-2),MidPt(LCD_HEIGHT,bH),bW-2,bH);
      SetColorMode(LCDColor::BLACK);
    }
    CenterText(0,pos,c);
    SetColorMode(LCDColor::WHITE);
  }
}

// Displays to LCD the current character
// selection in the main game.
void NavigateCharacter() {
  screen.setFont(u8g2_font_logisoso16_tr);
  const uint8_t wordLen=GetWordLength();
  const uint8_t cW=((screen.getMaxCharWidth()*2)/3), // char width
                cH=screen.getMaxCharHeight();        // char height
  const uint8_t nSpaces=(wordLen-1), // # spaces in between letters
                sp=LCD_CHAR_SPACING; // how big the space between letters should be (in pixels)
  const uint8_t tW=(cW*wordLen)+(nSpaces*sp); // total width
  const uint8_t cX=MidPt(LCD_WIDTH,tW),  // center x string offset
                cY=MidPt(LCD_HEIGHT,cH); // center y string offset
  uint8_t curX=cX;
  for (uint8_t i=0;i<wordLen;i++) {
    char c=arrangement[i];
    bool match=(i==GetHorizontalPos()),
         noExist=(c=='\0');
    c=(noExist?' ':c);
    if (match&&Flashing(1)) {
      screen.drawBox(curX+1,cY,cW-2,cH);
      screen.drawBox(curX,cY+1,cW,cH-2);
      SetColorMode(LCDColor::BLACK);
    }
    screen.drawGlyph(curX,cY+2,c);
    screen.drawGlyph(curX,cY+4,'_');
    curX+=(cW+sp);
    SetColorMode(LCDColor::WHITE);
  }
}

// Displays to LCD the in-game information
// on center top of the display (gamemode/difficulty).
void DisplayDifficulty() {
  const char* list[]={"WORDLE","SPEED","DEATHMATCH"};
  char sel[LCD_WIDTH/4]; // selected string from list[]
  const uint8_t i=static_cast<uint8_t>(GetMode()); // index of current gamemode
  snprintf(sel,sizeof(sel),"< %s >",list[i]);
  TopCenterText(sel);
}

// Displays to LCD the running
// in-game timer.
void DisplayTimer() {
  if (TimeRunning()) {
    const uint8_t m=GetMinutes(),
                  s=GetSeconds();
    BoldFont();
    // const int cH=screen.getMaxCharHeight();
    char timeArray[16]; // current string displaying time
    if (m>0) { sprintf(timeArray,"%dm %02ds",m,s); }
    else { sprintf(timeArray,"%ds",s); }
    TrackerText("time",timeArray);
  }
}

// Displays to LCD the lower-right
// HUD in the main game, for progress status.
void DisplayProgress() {
  SmallFont();
  // const int cHeight=screen.getMaxCharHeight();
  // int cWidth;
  char footer[16];
  if (CheckMode(Gamemode::SPEED)) {

  }
  
  else if (CheckMode(Gamemode::DEATHMATCH)) {

  }
  
  else {
    const uint8_t maxG=GetMaxGuesses(),
                  curG=GetWrongGuesses();
    snprintf(footer,sizeof(footer),"%d/%d",curG,maxG);
    TrackerText("guesses",footer);
  }

  if (CheckDiff(Difficulty::ENDLESS)) {
    size_t corG=GetCorrectGuesses();
    snprintf(footer,sizeof(footer),"%d correct",corG);
    PositionSub(footer,TextPos::BTM_RIGHT);
  }
}

void DisplaySecretWord() {
  const char* str="THE WORD IS ";
  void* sc=GetSecretWord();
  SecretWord* sec=(SecretWord*)sc;
  SmallFont(); const uint8_t cW=screen.getStrWidth(str);
  BoldFont(); const uint8_t cW2=screen.getStrWidth(sec->key);
  const uint8_t cH2=screen.getMaxCharHeight();
  const uint8_t tW=(cW+cW2);
  const uint8_t cX=MidPt(LCD_WIDTH,tW);
  const uint8_t posY=(LCD_HEIGHT-8);
  SmallFont(); screen.drawStr(cX,(posY-(cH2-(cH2/4))),str);
  BoldFont(); screen.drawStr((cX+cW),(posY-(cH2)),sec->key);
}

void DisplayPopup() {
  if (OnPopup()) {
    if (!CheckStatus(GameState::PLAYING)) {
      const bool endless=CheckDiff(Difficulty::ENDLESS),
                 hasWon=CheckStatus(GameState::WON),
                 gameover=(!hasWon||endless);
      const uint8_t x=(gameover?9:23),width=(gameover?108:81);
      screen.drawXBMP(x,8,width,34,(gameover?gameoverImg:youwinImg));
      DisplaySecretWord();
    } else {
      StraightLine(2,2); StraightLine(4,4);
      screen.drawXBMP(2,51,(LCD_WIDTH-4),11,loadingImg); // loading screen
    }
  }
}

void SetDisplayFlag(bool toggle) {
  displayFlag=toggle;
}

// ——————————————————————————————————————————————— //
// ——————————————————— page displaying ——————————— //
// ——————————————————————————————————————————————— //

void Animate_Intro() {
  const char* str="PRESS [BLUE] TO PLAY.";
  screen.drawXBMP(15,14,98,28,logoImg); SmallFont();
  const uint8_t cH=(screen.getMaxCharHeight()/2),
                cW=screen.getStrWidth(str);
  if (Flashing(1.2)) {
    screen.drawStr(MidPt(LCD_WIDTH,cW),
      (LCD_HEIGHT-(cH+5)),str);
  }
}

// Displays the 
void LCD_Menu() {
  // BigText(2,1,"MENU");
  screen.drawXBMP(2,2,(LCD_WIDTH-2),7,menuImg);
  NavigateSelect(LENG(MENU_BUTTONS),MENU_BUTTONS);
  if (Flashing(1.2)) {
    screen.drawXBMP(2,(LCD_HEIGHT-7),(LCD_WIDTH-4),5,menuNav);
  }
}

// Displays the gamemode menu after LCD_Menu().
void LCD_Gamemode() {
  const char* list[]={"WORDLE","SPEED","DEATHMATCH"};
  const uint8_t _m=static_cast<uint8_t>(Gamemode::numGamemodes);
  NavigateSelect(_m,list); StraightLine(2,2); StraightLine(4,4);
  PositionSub("(1/2) - SELECT GAMEMODE",TextPos::TOP_LEFT);
  if (Flashing(1.2)) {
    screen.drawXBMP(2,(LCD_HEIGHT-7),(LCD_WIDTH-4),5,selectNav);
  }
}

// displays the difficulty menu after LCD_Gamemode(). //
void LCD_Difficulty() {
  const char* list[]={"EASY","MODERATE",
          "EXPERT","HARDCORE","ENDLESS"};
  const uint8_t _m=static_cast<uint8_t>(Difficulty::numDiffs);
  NavigateSelect(_m,list); StraightLine(2,2); StraightLine(4,4);
  PositionSub("(2/2) - SELECT DIFFICULTY",TextPos::TOP_LEFT);
  if (Flashing(1.2)) { ShowNav(selectNav); }
}

// displays the current game statistics to the LCD. //
void LCD_Game() {
  NavigateCharacter();
  DisplayDifficulty();
  DisplayTimer();
  DisplayProgress();
  if (OnLocked()) {
    SmallFont();
    if (!GetLastWordValid()) {
      const char* txt="! INVALID !";
      const uint8_t cW=screen.getStrWidth(txt),
                    cH=screen.getMaxCharHeight();
      SmallText(MidPt(LCD_WIDTH,cW),LCD_HEIGHT-(cH+2),txt);
    }
  }
}

// displays the guide navigation page to the LCD. //
void LCD_GuidePage() {
  const uint8_t nPages=NUM_GUIDE_PAGES;
  screen.drawXBMP(2,2,(LCD_WIDTH-2),9,guideImg);
  char ft[16]; // footer
  snprintf(ft,sizeof(ft),"PAGE %d OF %d",GetHorizontalPos()+1,nPages);
  PositionSub(ft,TextPos::TOP_RIGHT);
  if (Flashing(1.2)) { ShowNav(seekNav); }
}

void LCD_StatsPage() {
  screen.drawXBMP(2,2,(LCD_WIDTH-2),7,statsImg);
  if (Flashing(1.2)) { ShowNav(statsNav); }
}

void LCD_RanksPage() {
  screen.drawXBMP(2,2,(LCD_WIDTH-2),7,ranksImg);
  if (Flashing(1.2)) { ShowNav(ranksNav); }
}

void LCD_ConfigPage() {
  screen.drawXBMP(2,2,(LCD_WIDTH-2),9,configImg);
  if (Flashing(1.2)) { ShowNav(seekNav); }
}

void LCD_Results() {
  StraightLine(2,2); StraightLine(4,4);
  PositionSub("RESULTS",TextPos::TOP_LEFT);
  if (Flashing(1.2)) {
    ShowNav(statsNav);
  }
}

void Navigate_LCD() {
  if (OnPopup()) {
    DisplayPopup();
  } else {
    // displays the contents of the page 
    // to the LCD by the page number.
    switch(GetPageNum()) {
      // Main title //
      case Page::TITLE: Animate_Intro(); break;

      // Menu directory //
      case Page::MAIN_MENU: LCD_Menu(); break;
      case Page::GUIDE: LCD_GuidePage(); break;
      case Page::STATS: LCD_StatsPage(); break;
      case Page::RANKS: LCD_RanksPage(); break;
      case Page::CONFIG: LCD_ConfigPage(); break;

      // Main game directory //
      case Page::GAMEMODE_SELECT: LCD_Gamemode(); break;
      case Page::DIFF_SELECT: LCD_Difficulty(); break;
      case Page::GAME: LCD_Game(); break;
      case Page::RESULTS: LCD_Results(); break;

      default: break;
    }
  }
}