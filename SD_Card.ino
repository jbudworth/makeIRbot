
void validateFilename() {
  bitClear(flags, 2);
  // First char is NULL?
  if (lastFile[0] != 0x00) {
    // Ignore hidden dot files
    if (lastFile[0] != 0x2E) {
      for (int i = 0; i < 32; i++) {
        // Watch for the period signifying the file extension
        if (lastFile[i] == 0x2E) {
          // Can we match the entire file extension?
          if (lastFile[i+1] == validExt[1] && 
              lastFile[i+2] == validExt[2] && 
              lastFile[i+3] == validExt[3]) {
            bitSet(flags, 2);
          }
        }
      }
    }
  }
}

void printFilename() {
  clearLCD(1);
  clearLCD(2);
  lcd.setCursor(0, 1);
  if (lastFile[0] == 0x00) {
    lcd.print("<NULL>");
  }
  else {
    for (int i = 0; i < 20; i++) {
      if (lastFile[i] == 0x00) {break;}
        lcd.write(lastFile[i]);
      }
    lcd.setCursor(0, 2);
    for (int i = 20; i < 32; i++) {
      if (lastFile[i] == 0x00) {break;}
        lcd.write(lastFile[i]);
      }
  }

  // Indicate valid file or not
  lcd.setCursor(numCols - 2, 3);
  if (bitRead(flags, 2)) {
    lcd.print("OK");
  }
  else {
    lcd.print("--");
  }
}

void playbackFile(uint8_t *filename) {
  int i = 0;
  uint8_t data[32];
  data[0] = 0x10; // Build command code
  while(filename[i] != 0x00) {
    data[i + 1] = filename[i];
    i++;
  }
  data[i + 1] = 0x00; // Add null termination
  sendBytesWithCRC(data, i + 2);
}

