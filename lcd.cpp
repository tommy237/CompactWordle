// —————— headers ————————————————————————— //
#include "lcd.h"
// —————— headers ————————————————————————— //


// —————— global vars ————————————————————— //
struct SecretWord { char key[10]; };
U8G2_ST7920_128X64_1_SW_SPI screen(U8G2_R0, E_PIN, RW_PIN, RS_PIN);
const uint8_t disp_width  = LCD_WIDTH, 
              disp_height = LCD_HEIGHT;
const uint8_t middle_X = (disp_width / 2),
              middle_Y = (disp_height / 2);
bool displayFlag = false;
const char* MENU_BUTTONS[] = {"PLAY", "GUIDE", "STATS", "RANKS", "CONFIG"};
unsigned long last_lock_ms = 0;
unsigned int last_delay = 0;
// —————— global vars ————————————————————— //



// |————————————————————————————————————————————| //
// |————| MODIFIED U8g2 |———————————————————————| //

void Init_LCD()
{
  if (!screen.begin()) {
    Serial.println("LCD failed to initialize.");
    while (1);
  }
  screen.setFontRefHeightText();
  screen.setFontPosTop();
}

void SmallFont() { screen.setFont(u8g2_font_tiny5_tr); }
void BoldFont() { screen.setFont(u8g2_font_helvB08_tr); }
void SetColorMode(LCDColor pte) { screen.setDrawColor(static_cast<uint8_t>(pte)); }
void ShowNav(const uint8_t* xbmp) { screen.drawXBMP(2, (disp_height - 7), (disp_width - 4), 5, xbmp); }
bool Flashing(double i) { return (millis() % (long)(i * 1E3)) < (i * 0.5E3); }
u8g2_uint_t MidPt(u8g2_uint_t pos1, u8g2_uint_t pos2) { return (pos1 - pos2) / 2; }

void BevelBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  screen.drawBox(x + 1, y, w - 2, h);
  screen.drawBox(x, y + 1, w, h - 2);
}

void CenterText(uint8_t x, uint8_t y, const char* str)
{
  const uint8_t string_width  = screen.getStrWidth(str), 
                string_height = screen.getMaxCharHeight();
  const uint8_t center_X = MidPt(disp_width, string_width), 
                center_Y = MidPt(disp_height, string_height);
  screen.drawStr((center_X + x), center_Y + (string_height / 3) + y, str);
}

// Creates a text label with string centered on top of the display.
void TopCenterText(const char* str)
{
  SmallFont();
  const uint8_t char_width = screen.getStrWidth(str), 
                char_height = (screen.getMaxCharHeight() / 2) - 3;
  const uint8_t center_X = MidPt(disp_width, char_width);
  screen.drawStr(center_X, char_height + 1, str);
}

// Creates a text label with a subtitle and target value,  often for in - game tracking
// or just to display the timer (jndicating loss or win for the game).
void TrackerText(const char* subttl, const char* str)
{
  BoldFont();
  const uint8_t char_height = screen.getMaxCharHeight();
  screen.drawStr(2, disp_height - (char_height), str);
  SmallFont();
  const uint8_t nH = screen.getMaxCharHeight();
  screen.drawStr(2, disp_height - (char_height + nH), subttl);
}

// Shorthand version of the u8g2.drawTriangle() with three
// int arrays as coordiante parameters.
void MakeArrow(uint8_t pt1[2], uint8_t pt2[2], uint8_t pt3[2])
{
  screen.drawTriangle(
    pt1[0], pt1[1],  
    pt2[0], pt2[1],  
    pt3[0], pt3[1]);
}

void BigText(uint8_t poX, uint8_t poY, const char* str)
{
  BoldFont();
  // const int char_height = screen.getMaxCharHeight();
  const uint8_t string_width = screen.getStrWidth(str), 
                center_X = MidPt(disp_width, string_width);
  screen.drawStr(poX, poY, str);
}

void SmallText(uint8_t poX, uint8_t poY, const char* str)
{
  SmallFont();
  const uint8_t char_height = screen.getMaxCharHeight();
  screen.drawStr(poX, (poY - char_height), str);
}

void StraightLine(const uint8_t h, const uint8_t padding)
{
  screen.drawLine(padding, h, (disp_width - (padding + 1)), h);
}

void PositionSub(const char* str, TextPos pos)
{
  const bool right = ((pos == TextPos::TOP_RIGHT) 
                   || (pos == TextPos::BTM_RIGHT)), 
             down  = ((pos == TextPos::BTM_LEFT)  
                   || (pos == TextPos::BTM_RIGHT));
  SmallFont();
  const uint8_t char_width  = screen.getStrWidth(str), 
                char_height = screen.getMaxCharHeight();
  const uint8_t x = right ? ((disp_width) - (char_width + 2)) : 2, 
                y = down  ? (disp_height - 1)                 : (char_height + 1);
  SetColorMode(LCDColor::BLACK);
  const uint8_t box_x = (x - 2),
                box_y = down ? (disp_height - (char_height + 1)) : 0,
                box_width = (char_width + 4),
                box_height = (char_height + 1);
  screen.drawBox(box_x, box_y, box_width, box_height);
  SetColorMode(LCDColor::WHITE);
  SmallText(x, y, str);
}

// |————| MODIFIED U8g2 |———————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| DISPLAY FUNCTIONS |———————————————————| //

bool OnPopup() { return displayFlag; }

// Displays to LCD the current options of a
// row of buttons to press on confirm.
void NavigateSelect(size_t maximum, const char* selects[])
{
  screen.setFont(u8g2_font_tiny5duo_te);

  const uint8_t box_height      = 8,
                height_spacing  = 7;

  const uint8_t arrow_padding   = 4, 
                arrow_spacing   = 8, 
                arrow_thickness = 4;

  for (size_t i = 0; i < maximum; i++) {
    const uint8_t h_pos = GetHorizontalPos();
    const char* c = selects[i];
    const int8_t diff = (i - h_pos);
    const bool match = (diff == 0); // if index matches current option
    int8_t pos = (height_spacing * diff);
    if (abs(diff) > 2) continue; // omit further options until 2 positions closer to index
    const bool below_index = (diff > 0); // pos starts from top to bottom; checking if item is below selected index
    pos = !match ? (below_index ? (pos + 1) : (pos - 2)) : pos; // spacing from matching index
    if (match) {
      const uint8_t box_width = screen.getStrWidth(c) + arrow_padding;
      const uint8_t arrow_back_h_pos = MidPt(disp_width, 0) - (box_width / 2) - arrow_spacing, 
                    mid_arrow_height = MidPt(disp_height, 0);
      screen.drawTriangle(arrow_back_h_pos,                   mid_arrow_height - (box_height / 2),   // top back
                          arrow_back_h_pos,                   mid_arrow_height + (box_height / 2),   // bottom back
                          arrow_back_h_pos + arrow_thickness, mid_arrow_height);                     // pointy tip
      // box dimensions
      BevelBox(MidPt(disp_width, box_width), 
               MidPt(disp_height, box_height),
               box_width, 
               box_height);
      // screen.drawBox(MidPt(disp_width, box_width),
      //                MidPt(disp_height, box_height - 2),
      //                box_width,
      //                box_height - 2);
      // screen.drawBox(MidPt(disp_width, box_width - 2),
      //                MidPt(disp_height, box_height),
      //                box_width - 2,
      //                box_height);
      SetColorMode(LCDColor::BLACK);
    }
    CenterText(0, pos, c);
    SetColorMode(LCDColor::WHITE);
  }
}

// Displays to LCD the current character
// selection in the main game.
void NavigateCharacter()
{
  screen.setFont(u8g2_font_logisoso16_tr);
  const uint8_t wordLen = GetWordLength();
  const uint8_t char_width  = ((screen.getMaxCharWidth() * 2) / 3),  // char width
                char_height = screen.getMaxCharHeight();             // char height
  const uint8_t nSpaces = (wordLen - 1),  // # spaces in between letters
                spacing = LCD_CHAR_SPACING; // how big the space between letters should be (in pixels)
  const uint8_t total_width = (char_width * wordLen) + (nSpaces * spacing); // total width
  const uint8_t center_X = MidPt(disp_width,  total_width),   // center x string offset
                center_Y = MidPt(disp_height, char_height); // center y string offset
  uint8_t current_X = center_X;
  for (uint8_t i = 0; i < wordLen; i++) {
    char c = arrangement[i];
    bool match = (i == GetHorizontalPos()), 
         noExist = (c == '\0');
    c = (noExist ? ' ' : c);
    if (match && Flashing(1)) {
      // screen.drawBox(current_X + 1, center_Y, char_width - 2, char_height);
      // screen.drawBox(current_X, center_Y + 1, char_width, char_height - 2);
      BevelBox(current_X, center_Y, char_width, char_height);
      SetColorMode(LCDColor::BLACK);
    }
    screen.drawGlyph(current_X, center_Y + 2, c);
    screen.drawGlyph(current_X, center_Y + 4, '_');
    current_X += (char_width + spacing);
    SetColorMode(LCDColor::WHITE);
  }
}

// Displays to LCD the in - game information
// on center top of the display (gamemode/difficulty).
void DisplayDifficulty()
{
  const char* list[] = {"WORDLE", "SPEED", "DEATHMATCH"};
  char selected[disp_width / 4];
  const uint8_t i = static_cast<uint8_t>(GetMode()); // index of current gamemode
  snprintf(selected, sizeof(selected), "< %s >", list[i]);
  TopCenterText(selected);
}

// Displays to LCD the running
// in - game timer.
void DisplayTimer()
{
  if (TimeRunning()) {
    const uint8_t m = GetMinutes(), 
                  s = GetSeconds();
    BoldFont();
    // const int char_height = screen.getMaxCharHeight();
    char timeArray[16]; // current string displaying time
    if (m > 0) { sprintf(timeArray, "%dm %02ds", m, s); }
    else { sprintf(timeArray, "%ds", s); }
    TrackerText("time", timeArray);
  }
}

// Displays to LCD the lower - right
// HUD in the main game,  for progress status.
void DisplayProgress()
{
  SmallFont();
  // const int char_heighteight = screen.getMaxCharHeight();
  // int char_widthidth;
  char footer[16];
  if (CheckMode(Gamemode::SPEED)) {

  }
  
  else if (CheckMode(Gamemode::DEATHMATCH)) {

  }
  
  else {
    const uint8_t maxG = GetMaxGuesses(), 
                  curG = GetWrongGuesses();
    snprintf(footer, sizeof(footer), "%d/%d", curG, maxG);
    TrackerText("guesses", footer);
  }

  if (CheckDiff(Difficulty::ENDLESS)) {
    size_t corG = GetCorrectGuesses();
    snprintf(footer, sizeof(footer), "%d correct", corG);
    PositionSub(footer, TextPos::BTM_RIGHT);
  }
}

void DisplaySecretWord()
{
  const char* str = "THE WORD IS ";
  void* sc = GetSecretWord();
  SecretWord* sec = (SecretWord*)sc;
  SmallFont(); const uint8_t char_width = screen.getStrWidth(str);
  BoldFont(); const uint8_t char_width2 = screen.getStrWidth(sec->key);
  const uint8_t char_height2 = screen.getMaxCharHeight(),
                total_width = (char_width + char_width2),
                center_X = MidPt(disp_width, total_width),
                posY = (disp_height - 8);
  SmallFont(); screen.drawStr(center_X, (posY - (char_height2 - (char_height2 / 4))), str);
  BoldFont(); screen.drawStr((center_X + char_width), (posY - char_height2), sec->key);
}

void DisplayPopup()
{
  if (OnPopup()) {
    if (!CheckStatus(GameState::PLAYING)) {
      const uint8_t gameover_pos_X =  9, gameover_width = 108,
                    youwin_pos_X   = 23, youwin_width   = 81;
      const bool endless  = CheckDiff(Difficulty::ENDLESS), 
                 has_won   = CheckStatus(GameState::WON), 
                 gameover = (!has_won || endless);
      const uint8_t x     = (gameover ? gameover_pos_X : youwin_pos_X), 
                    width = (gameover ? gameover_width : youwin_width);
      screen.drawXBMP(x, 8, width, 34, (gameover ? gameoverImg : youwinImg));
      DisplaySecretWord();
    } else {
      StraightLine(2, 2); StraightLine(4, 4);
      screen.drawXBMP(2, 51, (disp_width - 4), 11, loadingImg); // loading screen
    }
  }
}

void SetDisplayFlag(bool toggle)
{
  displayFlag = toggle;
}

// |————| DISPLAY FUNCTIONS |———————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| SCREEN LOCKING |——————————————————————| //

void UpdateLockTime(int del) 
{
  last_lock_ms = millis();
  last_delay = del;
}

void AuthorPrompt(int prompt_delay) 
{
  if (!OnPopup()) { 
    SetDisplayFlag(true);
    UpdateLockTime(prompt_delay);
  }
}

void SetLock(int lock_delay) 
{
  if (!OnLocked()) {
    LockButtons(true);
    UpdateLockTime(lock_delay);
  }
}

void OnLockWait() 
{
  // const uint16_t to_seconds = 1E3;
  const unsigned long ms = millis(), 
                      diff_ms = (ms - last_lock_ms);
  if (diff_ms > TO_SECONDS(last_delay)) {;
    if (OnPopup()) { SetDisplayFlag(false); } // locking the game
    else if (OnLocked()) { LockButtons(false); } // locking the buttons
  }
}

// |————| SCREEN LOCKING |——————————————————————| //
// |————————————————————————————————————————————| //



// |————————————————————————————————————————————| //
// |————| PAGE FUNCTIONS |——————————————————————| //

void Animate_Intro()
{
  const char* str = "PRESS [BLUE] TO PLAY.";
  screen.drawXBMP(15, 14, 98, 28, logoImg); SmallFont();
  const uint8_t char_height = (screen.getMaxCharHeight()/2), 
                char_width = screen.getStrWidth(str);
  if (Flashing(1.2)) {
    screen.drawStr(MidPt(disp_width, char_width), 
      (disp_height - (char_height + 5)), str);
  }
}

// Displays the 
void LCD_Menu()
{
  // BigText(2, 1, "MENU");
  screen.drawXBMP(2, 2, (disp_width - 2), 7, menuImg);
  NavigateSelect(LENG(MENU_BUTTONS), MENU_BUTTONS);
  if (Flashing(1.2)) { ShowNav(menuNav); }
}

// Displays the gamemode menu after LCD_Menu().
void LCD_Gamemode()
{
  const char* list[] = {"WORDLE", "SPEED", "DEATHMATCH"};
  const uint8_t amount = static_cast<uint8_t>(Gamemode::numGamemodes);
  NavigateSelect(amount, list);
  StraightLine(2, 2);
  StraightLine(4, 4);
  PositionSub("1/2 - SELECT GAMEMODE", TextPos::TOP_LEFT);
  if (Flashing(1.2)) { ShowNav(selectNav); }
}

// displays the difficulty menu after LCD_Gamemode(). //
void LCD_Difficulty()
{
  const char* list[] = {"EASY", "MODERATE", 
          "EXPERT", "HARDCORE", "ENDLESS"};
  const uint8_t amount = static_cast<uint8_t>(Difficulty::numDiffs);
  NavigateSelect(amount, list);
  StraightLine(2, 2);
  StraightLine(4, 4);
  PositionSub("2/2 - SELECT DIFFICULTY", TextPos::TOP_LEFT);
  if (Flashing(1.2)) { ShowNav(selectNav); }
}

// displays the current game statistics to the LCD. //
void LCD_Game()
{
  NavigateCharacter();
  DisplayDifficulty();
  DisplayTimer();
  DisplayProgress();
  if (OnLocked()) {
    SmallFont();

    if (!GetLastWordValid()) {
      const char* txt = "! INVALID !";
      const uint8_t char_width = screen.getStrWidth(txt), 
                    char_height = screen.getMaxCharHeight();
      SmallText(MidPt(disp_width, char_width), disp_height - (char_height + 2), txt);
    }
    
    // Displays the indicators as symbols.
    else {

    }
  }
}

// displays the guide navigation page to the LCD. //
void LCD_GuidePage()
{
  const uint8_t nPages = NUM_GUIDE_PAGES;
  screen.drawXBMP(2, 2, (disp_width - 2), 9, guideImg);
  char footer[16]; // footer
  snprintf(footer, sizeof(footer), "PAGE %d OF %d", GetHorizontalPos() + 1, nPages);
  PositionSub(footer, TextPos::TOP_RIGHT);
  if (Flashing(1.2)) { ShowNav(seekNav); }
}

void LCD_StatsPage()
{
  screen.drawXBMP(2, 2, (disp_width - 2), 7, statsImg);
  if (Flashing(1.2)) { ShowNav(statsNav); }
}

void LCD_RanksPage()
{
  screen.drawXBMP(2, 2, (disp_width - 2), 7, ranksImg);
  if (Flashing(1.2)) { ShowNav(ranksNav); }
}

void LCD_ConfigPage()
{
  screen.drawXBMP(2, 2, (disp_width - 2), 9, configImg);
  if (Flashing(1.2)) { ShowNav(seekNav); }
}

void LCD_Results()
{
  const uint8_t max_display = 5;
  void* sc = GetSecretWord();
  SecretWord* sec = (SecretWord*)sc;
  Entry list[] = {
    MakeEntry("ROUND", GetTotalRounds()),
    MakeEntry("MODE", static_cast<uint8_t>(GetMode())),
    MakeEntry("DIFF", static_cast<uint8_t>(GetDiff())),
    MakeEntry("GUESSES", GetLocalGuesses()),
    MakeEntry("WORD", sec->key),
  };
  SmallFont();
  const uint8_t h_pos = GetHorizontalPos();

  // Changing display ranges
  const uint8_t stats_min = (h_pos >= max_display ? (h_pos - (max_display - 1)) : 0),
                stats_max = (h_pos >= max_display ? (h_pos + 1) : max_display);

  // Display text horizontal positioning
  const uint8_t left_side = (disp_width / 3),
                right_side = (disp_width / 2); // used later for displaying scores

  // Display text vertical positioning
  const uint8_t height_spacing = 1,
                char_height = screen.getMaxCharHeight(),
                total_height = (stats_max * (char_height + height_spacing)),
                center_Y = MidPt(disp_height, total_height);

  uint8_t cur_Y = center_Y;
  for (uint8_t i = stats_min; i < stats_max; i++) {
    const uint8_t i_offset = (i - stats_min); // fixes the vertical layout alignment of each entry.
    const int8_t diff = (i - h_pos);
    const bool match = (diff == 0),
               below_index = (diff < i);
    int8_t pos = (height_spacing * diff);
    pos += match ? 0 : below_index ? 2 : -1;           

    // ——————— entry key display —————————————— //
    const char* c = list[i].key;
    const uint8_t str_width = screen.getStrWidth(c),
                  pos_X1 = (left_side - str_width);
    if (match) {
      const uint8_t padding_X = 10;
      const uint8_t box_width = (disp_width - (padding_X * 2)), 
                    box_height = (char_height + 1);
      screen.drawBox(padding_X, (cur_Y - 1), box_width, box_height);
      SetColorMode(LCDColor::BLACK);
    }
    screen.drawStr(pos_X1, cur_Y, c);
    // ——————— entry key display —————————————— //

    // ——————— entry value display —————————————— //
    const EntryType _type = list[i].type;
    char val_str[16];
    const char* val_ptr;
    switch(_type) {
      case EntryType::_INT: {
        snprintf(val_str, sizeof(val_str), "%lu", list[i].data.value);
        val_ptr = val_str;
      } break;
      case EntryType::_STRING: {
        val_ptr = list[i].data.str;
      } break;
      default: break;
    }
    const uint8_t pos_X2 = right_side;
    screen.drawStr(pos_X2, cur_Y, val_str);
    // ——————— entry value display —————————————— //

    cur_Y += (height_spacing * i_offset);
    SetColorMode(LCDColor::WHITE);
  }

  StraightLine(2, 2);
  StraightLine(4, 4);
  PositionSub("RESULTS", TextPos::TOP_LEFT);
  if (Flashing(1.2)) {
    ShowNav(statsNav);
  }
}

void Navigate_LCD()
{
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

// |————| PAGE FUNCTIONS |——————————————————————| //
// |————————————————————————————————————————————| //