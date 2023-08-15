// Libraries 
#include <SPI.h>

String UID;
int totalItems = 0;

// Different states for the device
enum device
{
  WELCOME,
  MENU,
  SCAN,
  NAME_INPUT,
  CHECKLIST,
  REMOVE
};

// Starting state
enum device state = WELCOME;

void resetSPI(){
  pinMode(53, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(53, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(8, HIGH);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  MFRC522Setup();
  TFTSetup();
}

void loop() {
  resetSPI();
  if (state == WELCOME){
    welcomeScreen();
    state = MENU;
    delay(5000);
    clearScreen();
  }

  if (state == MENU){
    menuScreen();
  }

  if (state == SCAN){
    scanScreen();
  }
  if (state == NAME_INPUT){
    clearScreen();
    inputScreen();
  }
  if (state == CHECKLIST){
    clearScreen();
    showChecklist();
  }
  if (state == REMOVE){
    clearScreen();
    removeItemsScreen();
  }
  
}

