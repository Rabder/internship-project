#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library
#include <XPT2046_Touchscreen.h>


#define TFT_CS    53      // TFT CS  pin is connected to arduino pin 8
#define TFT_RST   9      // TFT RST pin is connected to arduino pin 9
#define TFT_DC    10     // TFT DC  pin is connected to arduino pin 10
#define TOUCH_CS 8

#define TS_MINX 350
#define TS_MINY 250
#define TS_MAXX 3900
#define TS_MAXY 3850

bool flag = true;

XPT2046_Touchscreen ts(TOUCH_CS);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const char Mobile_KB[3][13] PROGMEM = {
  {0, 13, 10, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
  {1, 12, 9, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
  {3, 10, 7, 'Z', 'X', 'C', 'V', 'B', 'N', 'M'},
};

const char Mobile_NumKeys[3][13] PROGMEM = {
  {0, 13, 10, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
  {0, 13, 10, '-', '/', ':', ';', '(', ')', '$', '&', '@', '"'},
  {5, 8, 5, '.', '\,', '?', '!', '\''}
};

const char Mobile_SymKeys[3][13] PROGMEM = {
  {0, 13, 10, '[', ']', '{', '}', '#', '%', '^', '*', '+', '='},
  {4, 9, 6, '_', '\\', '|', '~', '<', '>'}, //4
  {5, 8, 5, '.', '\,', '?', '!', '\''}
};

const char textLimit = 25;
char MyBuffer[textLimit];


void TFTSetup(){
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(1);
}

void clearScreen(){
  tft.fillScreen(ILI9341_WHITE);
}


void testText() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
}


// 0 for x axis, 1 for y axis.
int getCenter(String text, int xy){
  int16_t x, y; 
  uint16_t width, height; 
  tft.getTextBounds(text, 0, 0, &x, &y, &width, &height);
  if (xy == 0){
    return (width / 2);
  }
  return (height / 2);
  
}

void drawArch(int16_t x, int16_t y, int16_t radius, int16_t startAngle, int16_t endAngle, uint16_t color) {
  for (int16_t angle = startAngle; angle <= endAngle; angle++) {
    int16_t x_pos = x + (int16_t)(radius * cos((angle - 90) * 0.0174533)); // Convert angle to radians
    int16_t y_pos = y + (int16_t)(radius * sin((angle - 90) * 0.0174533));
    tft.drawPixel(x_pos, y_pos, color);
  }
}

void menuScreen() {
  XPT2046Setup();
  bool flag = false;
  int x = 160 - 70;
  int y = 60;
  int textX;
  int textY;
  String buttonText[] = {"Checklist", "Scan new item", "Remove item"};
  for (int i = 0; i < 3; i++){
    drawButton(x, y, 140, 30, ILI9341_BLACK, ILI9341_BLACK, ILI9341_BLACK);
    tft.setCursor(x + 30, y + 10);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.println(buttonText[i]);
    y += 40;
  }
  while (true){
    if (TouchButton(x, 60, 140, 30)){
       Serial.println("CHECKLIST");
    state = CHECKLIST;
    break;
    }
    else if (TouchButton(x, 100, 140, 30)){
      Serial.println("SCAN");
      state = SCAN;
      break;
    }
    else if (TouchButton(x, 140, 140, 30)){
       Serial.println("IDK");
      state = IDK;
      break;
    }
  }
}

void welcomeScreen(){
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_BLACK);  
  int x = 160 - getCenter("Checklist!", 0);
  int y = 120 - getCenter("Checklist!", 1);
  tft.setCursor(x,y);
  tft.println("Checklist!");
  state = SCAN;
}

void scanScreenAnimation(){
  tft.fillCircle(150, 140, 5, ILI9341_BLACK);
  delay(500);
  drawArch(150, 140, 20, -80, 80, ILI9341_BLACK);
  delay(500);
  drawArch(150, 140, 35, -80, 80, ILI9341_BLACK);
  delay(500);
  drawArch(150, 140, 50, -80, 80, ILI9341_BLACK);
  delay(500);
  drawArch(150, 140, 20, -80, 80, ILI9341_WHITE);
  drawArch(150, 140, 35, -80, 80, ILI9341_WHITE);
  drawArch(150, 140, 50, -80, 80, ILI9341_WHITE);
}


void itemAlreadyExistsScreen(){
  
    String text = "Tag has already been scanned!";
    tft.fillCircle(150, 140, 5, ILI9341_WHITE);
    tft.setTextSize(2);
    int x = 160 - getCenter(text, 0);
    int y = 120 - getCenter(text, 1);
    tft.setCursor(x,y);
    tft.setTextColor(ILI9341_BLACK);  
    tft.println(text);
}

// First line of scan screen should be on the top left

void scanScreen(){
  clearScreen();
  String text = "Bring a tag close to the \n sensor to scan an item";
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK);
  int x = 160 - getCenter(text, 0);
  tft.setCursor(x, 5);
  tft.println(text);
  while(!readTag()){
    scanScreenAnimation();
  }
  state = NAME_INPUT;
}

void continueOrStop(){
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  String line1 = "Continue scanning?";
  clearScreen();
  tft.setCursor(5, 5);
  tft.println(line1);
  if (confirmationButtons()) {
      state = SCAN;
  }
    else{
      state =  CHECKLIST;
    }
}

void inputScreen(){
  // Get most recently scanned UID
  MakeKB_Button(Mobile_KB);
  XPT2046Setup();
  while(flag){
    GetKeyPress(MyBuffer);
  }
  confirmationScreen();
}

  void showChecklist(){
    
    String text = "Items";
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK);
    int16_t x = 5;
    int16_t y = 5;
    clearScreen();
    for (int i = 0; i < totalItems; i++){
      tft.setCursor(x + 30, y);
      tft.println(KEYS[i]);
      Serial.println(KEYS[i]);
      tft.setCursor(x + 50, y);
      drawButton(x, y, 20, 10, ILI9341_BLACK, ILI9341_BLACK, ILI9341_BLACK);
      y += 20;
    }
    scanChecklist();
  }

 void setCursor(int16_t x, int16_t y){
   tft.setCursor(x, y);
 }

  void confirmationScreen(){
    
    String text = String(MyBuffer);
    String value = UID;
    for (byte i = textLimit; i <= 0; i--){
        MyBuffer[i] = 0;
      }
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    String line1 = "Item scanned: " + text;
    String line2 = "UID: " + value;
    String line3 = "Do you wish to add this \n to the list?";
    int x1 = 160 - getCenter(line1, 0);
    int x2 = 160 - getCenter(line2, 0);
    int x3 = 160 - getCenter(line3, 0);
    clearScreen();
    tft.setCursor(5, 5);
    tft.println(line1);
    tft.setCursor(5, 30);
    tft.println(line2);
    tft.setCursor(5, 55);
    tft.println(line3);
    flag = true;
    if (confirmationButtons()) {
      insert(text, UID);
      totalItems++;
      UID = "";
    }
    else{
      UID = "";
    }
    continueOrStop();
  }


