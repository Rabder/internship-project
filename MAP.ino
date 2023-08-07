#define CAPACITY 10

String KEYS[CAPACITY];
String VALUES[CAPACITY];
String scanned[CAPACITY];
int size = 0;
int sizeScanned = 0;

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

void elim(String key) {
    for (int i = 0; i < size; i++) {
        if (KEYS[i].equals(key)) {
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
