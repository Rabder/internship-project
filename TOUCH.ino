#include <XPT2046_Touchscreen.h>

#define TFT_CS    53      // TFT CS  pin is connected to arduino pin 8
#define TFT_RST   9      // TFT RST pin is connected to arduino pin 9
#define TFT_DC    10     // TFT DC  pin is connected to arduino pin 10
#define TOUCH_CS 24

#define coordOffset 


void XPT2046Setup(){
  ts.begin();
  ts.setRotation(1);
}

// Keyboard prep stuff starts here

#define IsWithin(x, a, b) ((x>=a)&&(x<=b))

// Keyboard prep stuff ends here

void drawButton(int x, int y, int w, int h, uint16_t shading, uint16_t outter, uint16_t inner)
{
  // grey
  tft.fillRoundRect(x - 3, y + 3, w, h, 3, shading); //Button Shading

  // white
  tft.fillRoundRect(x, y, w, h, 3, outter); // outter button color

  //red
  tft.fillRoundRect(x + 1, y + 1, w - 1 * 2, h - 1 * 2, 3, inner); //inner button color
}


void MakeKB_Button(const char type[][13])
{
  tft.setTextSize(2);
  tft.setTextColor(0xffff, 0xf000);
  for (int y = 0; y < 3; y++)
  {
    int ShiftRight = 15 * pgm_read_byte(&(type[y][0]));
    for (int x = 3; x < 13; x++)
    {
      if (x >= pgm_read_byte(&(type[y][1]))) break;

      drawButton(15 + (30 * (x - 3)) + ShiftRight, 100 + (30 * y), 20, 25, 0x8888, 0xffff, 0xf800); // this will draw the button on the screen by so many pixels
      tft.setCursor(20 + (30 * (x - 3)) + ShiftRight, 105 + (30 * y));
      tft.print(char(pgm_read_byte(&(type[y][x]))));
    }
  }
  //ShiftKey
  drawButton(15, 160, 35, 25, 0x8888, 0xffff, 0xf800);
  tft.setCursor(27, 168);
  tft.print('^');

  //Special Characters
  drawButton(15, 190, 35, 25, 0x8888, 0xffff, 0xf800);
  tft.setCursor(21, 195);
  tft.print(F("SP"));

  //BackSpace
  drawButton(270, 160, 35, 25, 0x8888, 0xffff, 0xf800);
  tft.setCursor(276, 165);
  tft.print(F("BS"));

  //Return
  drawButton(270, 190, 35, 25, 0x8888, 0xffff, 0xf800);
  tft.setCursor(276, 195);
  tft.print(F("RT"));

  //Spacebar
  drawButton(60, 190, 200, 25, 0x8888, 0xffff, 0xf800);
  tft.setCursor(105, 195);
  tft.print(F("SPACE BAR"));
}

void GetKeyPress(char * textBuffer)
{

  char key = 0;
  static bool shift = false, special = false, back = false, lastSp = false, lastSh = false;
  static char bufIndex = 0;

    //ShiftKey
    if (TouchButton(15, 160, 35, 25))
    {
      Serial.println("SHIFT");
      shift = !shift;
      delay(100);
    }

    //Special Characters
    if (TouchButton(15, 190, 35, 25))
    {
      special = !special;
      delay(100);
    }

    if (special != lastSp || shift != lastSh)
    {
      if (special)
      {
        if (shift)
        {
          tft.fillScreen(ILI9341_WHITE);
          MakeKB_Button(Mobile_SymKeys);
        }
        else
        {
          tft.fillScreen(ILI9341_WHITE);
          MakeKB_Button(Mobile_NumKeys);
        }
      }
      else
      {
        tft.fillScreen(ILI9341_WHITE);
        MakeKB_Button(Mobile_KB);
        tft.setTextColor(0xffff, 0xf800);
      }

      if (special)
        tft.setTextColor(0x0FF0, 0xf800);
      else
        tft.setTextColor(0xFFFF, 0xf800);

      tft.setCursor(21, 195);
      tft.print(F("SP"));

      if (shift)
        tft.setTextColor(0x0FF0, 0xf800);
      else
        tft.setTextColor(0xffff, 0xf800);

      tft.setCursor(27, 168);
      tft.print('^');

      lastSh = shift;

      lastSp = special;
      lastSh = shift;
    }

    for (int y = 0; y < 3; y++)
    {
      int ShiftRight;
      if (special)
      {
        if (shift)
          ShiftRight = 15 * pgm_read_byte(&(Mobile_SymKeys[y][0]));
        else
          ShiftRight = 15 * pgm_read_byte(&(Mobile_NumKeys[y][0]));
      }
      else
        ShiftRight = 15 * pgm_read_byte(&(Mobile_KB[y][0]));

      for (int x = 3; x < 13; x++)
      {
        if (x >=  (special ? (shift ? pgm_read_byte(&(Mobile_SymKeys[y][1])) : pgm_read_byte(&(Mobile_NumKeys[y][1]))) : pgm_read_byte(&(Mobile_KB[y][1])) )) break;

        if (TouchButton(15 + (30 * (x - 3)) + ShiftRight, 100 + (30 * y), 20, 25)) // this will draw the button on the screen by so many pixels
        {
          if (bufIndex < (textLimit - 1))
          {
            delay(100);

            if (special)
            {
              if (shift)
                textBuffer[bufIndex] = pgm_read_byte(&(Mobile_SymKeys[y][x]));
              else
                textBuffer[bufIndex] = pgm_read_byte(&(Mobile_NumKeys[y][x]));
            }
            else
              textBuffer[bufIndex] = (pgm_read_byte(&(Mobile_KB[y][x])) + (shift ? 0 : ('a' - 'A')));
            bufIndex++;
          }
          break;
        }
      }
    }

    //Spacebar
    if (TouchButton(60, 190, 200, 25))
    {
      textBuffer[bufIndex++] = ' ';
      delay(100);
    }

    //BackSpace
    if (TouchButton(270, 160, 35, 25))
    {
      if ((bufIndex) > 0)
        bufIndex--;
      textBuffer[bufIndex] = 0;
      tft.setTextColor(0, ILI9341_WHITE);
      tft.setCursor(15, 80);
      tft.print(F("                       "));
      delay(100);
    }

    //Return
    if (TouchButton(270, 190, 35, 25) && UID.length() > 0 && bufIndex > 0)
    {
      flag = false;
      state = CHECKLIST;
      return;
    }

  tft.setTextColor(0xffff, 0xf800);
  tft.setCursor(15, 80);
  tft.print(textBuffer);
}

bool confirmationButtons(){
  tft.setTextColor(0xffff, 0x03e0);
  drawButton(120, 120, 60, 30, 0x8888, 0xffff, 0x03e0);
  tft.setCursor(130, 130);
  tft.print(F("Yes"));
  tft.setTextColor(0xffff, 0xf800);
  drawButton(120, 160, 60, 30, 0x8888, 0xffff, 0xf800);
  tft.setCursor(135, 170);
  tft.print(F("No"));
  return getKeyPressConfirmation();

}

bool getKeyPressConfirmation() {
  while (true){
    if (TouchButton(120, 120, 60, 30))
    {
      Serial.println("YES");
      return true;
    }
    else if (TouchButton(120, 160, 60, 30)) {
      Serial.println("NO");
      return false;
    }
  }
}


byte TouchButton(int x, int y, int w, int h)
{

  int X, Y;
  // Retrieve a point
  TS_Point p = ts.getPoint();
  if (p.z > 10){
    Y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    X = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(X);
    Serial.print(", y = ");
    Serial.print(Y);
    delay(30);
    Serial.println();

    return (IsWithin(X, x, x + w) & IsWithin(Y, y, y + h));
  }
  return 0;
}

