#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Keypad.h>


#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


const byte ROWS = 4;
const byte COLS = 4;

char Keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}
};

byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {3,  2, A4, A5};

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);


#define COLOR_BG     0x0000
#define COLOR_HEADER 0x1C9F
#define COLOR_SELECT 0x07E0
#define COLOR_TEXT   0xFFFF
#define COLOR_DIM    0xAD55
#define COLOR_DIV    0x4208


enum Screen {
  HOME,
  CONTACTS,
  ME,
  SETTINGS
};

Screen currentScreen  = HOME;
Screen previousScreen = HOME;
int selectedIndex     = 0;
int detailIndex       = 0;


const char* homeItems[] = {"Contacts", "Me", "Settings"};  
const int   homeCount   = 3;

const char* contactNames[] = {"David", "Samuel"};           
const int   contactCount   = 2;                            

const char* settingItems[] = {"Bluetooth"};                 
const int   settingCount   = 1;                           

const char* MeDetails[] = {"omoo", "I DID THIS", "STRESS", "I DID IT"};  
const int   MeCount     = 4;


void drawHeader(const char* title) {
  tft.fillRect(0, 0, 240, 45, COLOR_HEADER);
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(2);
  int tx = (240 - strlen(title) * 12) / 2;
  tft.setCursor(tx > 0 ? tx : 4, 14);
  tft.print(title);
  Serial.println("Header drawn");
}

void drawNavHint() {
  tft.setTextColor(COLOR_DIM);
  tft.setTextSize(1);
  tft.setCursor(10, 310);
  tft.print("2=Up  8=Dn  5=OK  *=Back");
  Serial.println("NavHint drawn");
}

void drawBackHint() {
  tft.setTextColor(COLOR_DIM);
  tft.setTextSize(1);
  tft.setCursor(10, 310);
  tft.print("* = Back");
  Serial.println("BackHint drawn");
}



void drawHome() {
  tft.fillScreen(COLOR_BG);
  tft.fillRect(0, 0, 240, 55, COLOR_HEADER);
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(3);
  tft.setCursor(48, 13);
  tft.print("My Phone");


  for (int i = 0; i < homeCount; i++) {
    int y = 72 + i * 68;
    uint16_t bg = (i == selectedIndex) ? COLOR_SELECT : COLOR_BG;
    tft.fillRect(10, y, 220, 56, bg);
    tft.drawRect(10, y, 220, 56, COLOR_HEADER);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    int tx = (240 - strlen(homeItems[i]) * 12) / 2;
    tft.setCursor(tx, y + 18);
    tft.print(homeItems[i]);
    Serial.println("Home drawn");
  }
  drawNavHint();
}

void drawContacts() {
  tft.fillScreen(COLOR_BG);
  drawHeader("Contacts");

  for (int i = 0; i < contactCount; i++) {   
    int y = 52 + i * 54;
    if (i == selectedIndex) tft.fillRect(0, y, 240, 52, COLOR_SELECT);
    tft.fillCircle(26, y + 20, 14, COLOR_HEADER);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(22, y + 16);
    tft.print(contactNames[i][0]);
    tft.setTextSize(2);
    tft.setCursor(48, y + 14);
    tft.print(contactNames[i]);
    tft.drawLine(0, y + 51, 240, y + 51, COLOR_DIV);
    Serial.println("contacts drawn");
  }
  drawNavHint();
}

void drawSettings() {
  tft.fillScreen(COLOR_BG);
  drawHeader("Settings");

  for (int i = 0; i < settingCount; i++) {   
    int y = 52 + i * 54;
    if (i == selectedIndex) tft.fillRect(0, y, 240, 52, COLOR_SELECT);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(15, y + 14);
    tft.print(settingItems[i]);              
    tft.setCursor(218, y + 14);
    tft.print(">");
    tft.drawLine(0, y + 51, 240, y + 51, COLOR_DIV);
    Serial.println("Settings drawn");
  }
  drawNavHint();
}

void drawMe() {
  tft.fillScreen(COLOR_BG);
  drawHeader("Me");

  tft.fillCircle(120, 110, 45, COLOR_HEADER);
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(3);
  tft.setCursor(107, 96);
  tft.print("Z");

  tft.setTextSize(2);
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(70, 165);
  tft.print("Zion Dev");

  tft.drawLine(10, 190, 230, 190, COLOR_DIV);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_DIM);
  tft.setCursor(15, 200);
  tft.print("Status");
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(2);
  tft.setCursor(15, 213);
  tft.print("zion and AI did this shit ");

  tft.drawLine(10, 238, 230, 238, COLOR_DIV);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_DIM);
  tft.setCursor(15, 248);
  tft.print("Phone");
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(2);
  tft.setCursor(15, 261);
  tft.print("+234 000 0000");
  Serial.println("Me drawn");

  drawBackHint();
}



void drawScreen(Screen s) {
  switch (s) {
    case HOME:     drawHome();     break;
    case CONTACTS: drawContacts(); break;
    case ME:       drawMe();       break;
    case SETTINGS: drawSettings(); break;
  }
}



int listCount(Screen s) {
  if (s == CONTACTS) return contactCount;
  if (s == ME)       return MeCount;
  if (s == SETTINGS) return settingCount;
  return homeCount;
}


bool isDetail(Screen s) {
  return false;
}

void handleKey(char key) {
  if (key == '*') {
    selectedIndex = 0;
    currentScreen = (currentScreen != HOME) ? HOME : HOME;  
    drawScreen(currentScreen);
    return;
  }

  int count = listCount(currentScreen);

  if (key == '2') {
    selectedIndex = (selectedIndex - 1 + count) % count;
    drawScreen(currentScreen);
  }
  else if (key == '8') {
    selectedIndex = (selectedIndex + 1) % count;
    drawScreen(currentScreen);
  }
  else if (key == '5') {
    detailIndex    = selectedIndex;
    previousScreen = currentScreen;
    if (currentScreen == HOME) {
      if      (selectedIndex == 0) currentScreen = CONTACTS;
      else if (selectedIndex == 1) currentScreen = ME;
      else if (selectedIndex == 2) currentScreen = SETTINGS;
      selectedIndex = 0;
    }
    drawScreen(currentScreen);
  }
}



void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(COLOR_BG);
  drawScreen(HOME);
}

void loop() {
  char key = customKeypad.getKey();   
  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);
    handleKey(key);
  }
}
