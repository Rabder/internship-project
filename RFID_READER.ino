// Libraries 
#include <SPI.h>
#include <MFRC522.h>

// RFID pins
#define RST_RFID 5
#define SS_RFID 53

MFRC522 RFID_READER(SS_RFID, RST_RFID);


void setup() {
  Serial.begin(9600);
  SPI.begin();
  RFID_READER.PCD_Init();
}

void loop() {

  if (!RFID_READER.PICC_IsNewCardPresent()){
    Serial.println("No tag found!");
    return;
  }

  if (!RFID_READER.PICC_ReadCardSerial()){
    Serial.println("Can't get data on tag!");
    return;
  }

  String UID;
  Serial.print("UID:");
  for (byte i = 0; i < RFID_READER.uid.size; i++){
    // If byte less than 0x10, print ' 0'. Print ' ' otherwise 
    Serial.print(RFID_READER.uid.uidByte[i] < 0x10 ? " 0": " ");
    Serial.print(RFID_READER.uid.uidByte[i], HEX);
    UID += String(RFID_READER.uid.uidByte[i], HEX);
  }
  Serial.println();

  insert(UID, "Wallet");
  Serial.println(lookup(UID)); // Should print 'Wallet'
  RFID_READER.PICC_HaltA();
}
