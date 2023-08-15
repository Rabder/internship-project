#include <EEPROM.h>

#define CAPACITY 10

String KEYS[CAPACITY];
String VALUES[CAPACITY];
String scanned[CAPACITY];
int size = 0;
int sizeScanned = 0;


void writeEEPROM(int addrOff, const String &str){
  byte length = str.length();
  EEPROM.write(addrOff, length);
  for (int i = 0; i < length; i++)
  {
    EEPROM.write(addrOff + 1 + i, str[i]);
  }
}

String readStringFromEEPROM(int addrOffset)
{
  int newaddrOff = addrOffset;
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\ 0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  addrOffset = newaddrOff;
  return String(data);
}

void insert(String key, String value) {
    // Check if the key already exists
    for (int i = 0; i < size; i++) {
        if (KEYS[i].equals(key)) {
            VALUES[i] = value; // Update value if the key already exists
            return;
        }
    }
    // Key doesn't exist, add it to the arrays
    KEYS[size] = key;
    VALUES[size] = value;
    size++;
}

String lookup(String key) {
    for (int i = 0; i < size; i++) {
        if (KEYS[i].equals(key)) {
            return VALUES[i]; // Return the value if the key is found
        }
    }
    // Key not found, return a space
    return " ";
}

void elim(String value) {
    for (int i = 0; i < size; i++) {
        if (VALUES[i].equals(value)) {
            // Shift elements to the left to fill the gap left by the deleted key
            for (int j = i; j < size - 1; j++) {
                KEYS[j] = KEYS[j + 1];
                VALUES[j] = VALUES[j + 1];
            }
            size--;
            return; // Exit the function after deletion
        }
    }
}

int checkIfDuplicate(String value, String* ARRAY){
  for (int i = 0; i < totalItems; i++){
    if (ARRAY[i].equals(value)){
      return i;
    }
  }
  return -1;
}

void resetChecklist(){
  sizeScanned = 0;
  for (int i = 0; i < CAPACITY; i++){
    scanned[i] = "\0";
  }
}
