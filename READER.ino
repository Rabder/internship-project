#include <MFRC522.h>

// RFID pins
#define RST_RFID 7
#define SS_RFID 11

MFRC522 RFID_READER(SS_RFID, RST_RFID);

void MFRC522Setup(){
  RFID_READER.PCD_Init();
}

bool readTag(){
    if (!RFID_READER.PICC_IsNewCardPresent()){
    Serial.println("No tag found!");
    return false;
  }

  if (!RFID_READER.PICC_ReadCardSerial()){
    Serial.println("Can't get data on tag!");
    return false;
  }

  else{
  Serial.print("UID:");
  for (byte i = 0; i < RFID_READER.uid.size; i++){
    // If byte less than 0x10, print ' 0'. Print ' ' otherwise 
    Serial.print(RFID_READER.uid.uidByte[i] < 0x10 ? " 0": " ");
    Serial.print(RFID_READER.uid.uidByte[i], HEX);
    UID += String(RFID_READER.uid.uidByte[i], HEX);
  }
  
  if (checkIfDuplicate(UID, VALUES) == -1 || totalItems < 1){
    Serial.println();
    RFID_READER.PICC_HaltA();
    return true;
  }
  else if (checkIfDuplicate(UID, VALUES) >= 0){
    Serial.println(checkIfDuplicate(UID, VALUES));
    clearScreen();
    itemAlreadyExistsScreen();
    UID = "";
    delay(5000);
    clearScreen();
    return false;
  }
  }
}


bool readTagChecklist(){
    if (!RFID_READER.PICC_IsNewCardPresent()){
    Serial.println("No tag found!");
    return false;
  }

  if (!RFID_READER.PICC_ReadCardSerial()){
    Serial.println("Can't get data on tag!");
    return false;
  }

  else{
  Serial.print("UID:");
  for (byte i = 0; i < RFID_READER.uid.size; i++){
    // If byte less than 0x10, print ' 0'. Print ' ' otherwise 
    Serial.print(RFID_READER.uid.uidByte[i] < 0x10 ? " 0": " ");
    Serial.print(RFID_READER.uid.uidByte[i], HEX);
    UID += String(RFID_READER.uid.uidByte[i], HEX);
  }
  
  if (checkIfDuplicate(UID, VALUES) == -1 || totalItems < 1){
    Serial.println();
    RFID_READER.PICC_HaltA();
    return true;
  }
  else if (checkIfDuplicate(UID, VALUES) >= 0){
    Serial.println(checkIfDuplicate(UID, VALUES));
    clearScreen();
    itemAlreadyExistsScreen();
    UID = "";
    delay(5000);
    clearScreen();
    return false;
  }
  }
}

 void scanChecklist(){
    int16_t x = 5;
    int16_t y = 5;
    int16_t newx;
    int16_t newy;
    while (true){
      if (RFID_READER.PICC_IsNewCardPresent() && RFID_READER.PICC_ReadCardSerial()){
      Serial.print("UID:");
      for (byte i = 0; i < RFID_READER.uid.size; i++){
        // If byte less than 0x10, print ' 0'. Print ' ' otherwise 
        Serial.print(RFID_READER.uid.uidByte[i] < 0x10 ? " 0": " ");
        Serial.print(RFID_READER.uid.uidByte[i], HEX);
        UID += String(RFID_READER.uid.uidByte[i], HEX);
        Serial.println();
      }

      Serial.print("VALUES: " );
        Serial.println(checkIfDuplicate(UID, VALUES));
        Serial.print("SCANNED: " );
        Serial.println(checkIfDuplicate(UID, scanned));
      
      if (checkIfDuplicate(UID, VALUES) != -1 && checkIfDuplicate(UID, scanned) == -1){
        Serial.println("VALID");
        newx = x;
        newy = y + checkIfDuplicate(UID, VALUES) * 20;
        setCursor(newx, newy);
        drawButton(newx, newy, 20, 10, 0x03e0, 0x03e0, 0x03e0);
        scanned[sizeScanned] = UID;
        sizeScanned++;
        
      }
      UID = "";
    }
  }
 }