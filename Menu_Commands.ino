//Menu Structure
void menuSetup() {

  menu.getRoot().add(m_connect);      // <-- Connect
  m_connect.addRight(m_flags);
  m_flags.addRight(m_debug);
  m_debug.addRight(m_debugtog);
  m_connect.addAfter(m_temps);        // <-- Temperature
  m_temps.addRight(m_heaters);
  m_heaters.addRight(m_extruder_temp);
  m_extruder_temp.addRight(m_hbp_temp);
  m_temps.addAfter(m_file);           // <-- File
  m_file.addRight(m_build);
  m_file.addAfter(m_position);        // <-- Position
  m_position.addRight(m_zero);
  m_zero.addRight(m_jog_unit);
  m_jog_unit.addRight(m_jogx);
  m_jogx.addRight(m_jogy);
  m_jogy.addRight(m_jogz);
  m_position.addAfter(m_extruder);    // <-- Extruder
  m_extruder.addRight(m_extruder_dir);     
  m_extruder_dir.addRight(m_extruder_rpm);     
  m_extruder.addAfter(m_fan);		  // <-- Fan
  m_fan.addAfter(m_reset);			  // <-- Reset

  m_extruder_temp.addBefore(m_extruder_temp_plus);
  m_extruder_temp.addAfter(m_extruder_temp_minus);

  m_hbp_temp.addBefore(m_hbp_temp_plus);
  m_hbp_temp.addAfter(m_hbp_temp_minus);

  m_heaters.addBefore(m_enable_heaters);
  m_heaters.addAfter(m_disable_heaters);

  m_jogx.addBefore(m_jogx_plus);
  m_jogx.addAfter(m_jogx_minus);

  m_jogy.addBefore(m_jogy_plus);
  m_jogy.addAfter(m_jogy_minus);

  m_jogz.addBefore(m_jogz_plus);
  m_jogz.addAfter(m_jogz_minus);

  m_extruder_dir.addBefore(m_extruder_rev);
  m_extruder_dir.addAfter(m_extruder_fwd);

  m_extruder_rpm.addBefore(m_extruder_rpm_plus);
  m_extruder_rpm.addAfter(m_extruder_rpm_minus);      

  //  Loop between first and last menu items.
  m_reset.addAfter(m_connect);
  m_connect.addBefore(m_reset);

}

// =============== Connect Menu ===============
void infoDisplay(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(19,0); lcd.write((uint8_t) 3);
  lcd.setCursor(0,0);
  if (bitRead(flags, 0)) {
    for (int i = 0; i < 16; i++) {
      if (machineName[i] != 0x00) {
        lcd.write(machineName[i]);
      }
      else {
        lcd.print(" ");
      }
    }
  }
  else {
    lcd.print("<NO CONNECTION>");
  }
  clearLCD(1);
  lcd.setCursor(0,1); lcd.print("MB Firmware v");lcd.print(MotherboardVersion,1);
  clearLCD(2);
  lcd.setCursor(0,2); lcd.print("EC Firmware v");lcd.print(ExtruderControllerVersion,1);
}
void flagDisplay(){
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  lcd.setCursor(0, 1);
  lcd.print("c"); 
  lcd.print(bitRead(flags, 0));
  lcd.print("b"); 
  lcd.print(bitRead(flags, 1));
  lcd.print("v"); 
  lcd.print(bitRead(flags, 2));
  lcd.print("-"); 
  lcd.print(bitRead(flags, 3));
  lcd.print("-"); 
  lcd.print(bitRead(flags, 4));
  lcd.print("-"); 
  lcd.print(bitRead(flags, 5));
  lcd.print("-"); 
  lcd.print(bitRead(flags, 6));
  lcd.print("d"); 
  lcd.print(bitRead(flags, 7));
}
void debugDisplay(){
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  lcd.setCursor(0, 1);
//  for (int i = 0; i < responseLength; i++) {
    for (int i = 0; i < 32; i++) {

    lcd.write(serialIn[i]);
  }
}
void debugtogDisplay(){
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(5);

}

// ============= Temperature Menu =============
void tempDisplay(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(19,0); lcd.write((uint8_t) 3);
  lcd.setCursor(1,1); lcd.print("Extruder "); printTemp(extTemp, 11, 1, 47);
  printTemp(extTarget, 15, 1, 223);
  lcd.setCursor(1,2); lcd.print("Platform "); printTemp(hbpTemp, 11, 2, 47);
  printTemp(hbpTarget, 15, 2, 223);
  clearLCD(3);
}
void heatersDisplay(){
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  lcd.setCursor(5,1); lcd.print("Enable "); lcd.write((uint8_t) 0);  
  lcd.setCursor(5,2); lcd.print("Disable"); lcd.write((uint8_t) 1);
}
void extruder_temp(){
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  printTemp(setextTemp, 8, 2, 223); 
  lcd.write((uint8_t) 4);

}
void hbp_temp(){
  lcd.setCursor(18,0); lcd.write(2);
  printTemp(sethbpTemp, 8, 2, 223);     
  lcd.write((uint8_t) 4);
}

// ================ File Menu ================
void fileSelect(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(17,0); lcd.write((uint8_t) 5);
  lcd.setCursor(19,0); lcd.write((uint8_t) 3);
  printFilename(); // Make sure the filename display is always updated
}
void fileBuild(){
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(17,0); lcd.write(5);

  if (!bitRead(flags, 2)) { // Valid filename?
    menu.moveLeft();
  }
}

// ============== Position Menu ==============
void positionDisplay(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(19,0); lcd.write((uint8_t) 3);
  clearLCD(1); lcd.setCursor(5,1); lcd.print("X:"); printPos(0, 1, 9); lcd.setCursor(15,1); lcd.print("  ");  
  clearLCD(2); lcd.setCursor(5,2); lcd.print("Y:"); printPos(1, 2, 9); lcd.setCursor(15,2); lcd.print("  ");  
  clearLCD(3); lcd.setCursor(5,3); lcd.print("Z:"); printPos(2, 3, 9); lcd.setCursor(15,3); lcd.print("  ");
}
void zeroposDisplay(){
  lcd.setCursor(17,0);  lcd.write(5);
  lcd.setCursor(18,0);  lcd.write(2);
  lcd.setCursor(19,0);  lcd.write(3);
}
void jogunitDisplay(){
  lcd.setCursor(17,0);  lcd.write(5);
  lcd.setCursor(18,0);  lcd.write(2);
  lcd.setCursor(19,0);  lcd.write(3);
  lcd.setCursor(8,2); lcd.print(jogDistance[jogIndex],1);lcd.print(" ");   lcd.setCursor(12,2); lcd.print(" mm "); lcd.write(5);

}
void jogxDisplay(){
  lcd.setCursor(0,0); lcd.print("  ");
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  lcd.setCursor(5,2); lcd.print("X: "); printPos(0, 2, 8); lcd.setCursor(13,2); lcd.print("       ");
  clearLCD(1);
  lcd.setCursor(9,1); lcd.write((uint8_t) 0);
  lcd.setCursor(9,3); lcd.write((uint8_t) 1);
}
void jogyDisplay(){
  lcd.setCursor(0,0); lcd.print("  ");
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  lcd.setCursor(5,2); lcd.print("Y: "); printPos(1, 2, 8); lcd.setCursor(13,2); lcd.print("       ");
  clearLCD(1);
  lcd.setCursor(9,1); lcd.write((uint8_t) 0);
  lcd.setCursor(9,3); lcd.write((uint8_t) 1);
}
void jogzDisplay(){
  lcd.setCursor(0,0); lcd.print("  ");
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(5,2); lcd.print("Z: "); printPos(2, 2, 8); lcd.setCursor(13,2); lcd.print("       ");
  clearLCD(1);
  lcd.setCursor(9,1); lcd.write((uint8_t) 0);
  lcd.setCursor(9,3); lcd.write((uint8_t) 1);
}

// ============== Extruder Menu =============
void extruder(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(19,0); lcd.write((uint8_t) 3); 
}
void extruder_dir(){
  lcd.setCursor(0,0);  lcd.print("  ");
  lcd.setCursor(17,0); lcd.write(5);
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(19,0); lcd.write(3);
  lcd.setCursor(5,1);  lcd.print("Reverse "); lcd.write((uint8_t) 0);
  lcd.setCursor(5,2);  lcd.print("Forward "); lcd.write((uint8_t) 1);
  lcd.setCursor(2,3);  lcd.print("Run Duration: "); lcd.print(extruder_duration[extruder_duration_Index]);lcd.print("s  ");
}
void extruder_dir_rev(){
  lcd.setCursor(0,0);  lcd.print("  ");
  lcd.setCursor(18,0);       lcd.write((uint8_t) 1);
  lcd.setCursor(6,1);        lcd.print("Reverse");   //   lcd.write((uint8_t) 0);
}
void extruder_dir_fwd(){
  lcd.setCursor(0,0); lcd.print("  ");
  lcd.setCursor(18,0); lcd.write((uint8_t) 0);
  lcd.setCursor(6,2); lcd.print("Forward");   //   lcd.write((uint8_t) 1);

}
void extruder_rpm() {
  lcd.setCursor(0,0); lcd.print("  ");
  lcd.setCursor(18,0); lcd.write(2);
  lcd.setCursor(5,2); lcd.print("RPM: ");   
  lcd.print(extruder_RPM); //lcd.setCursor(13,2); lcd.print("             ");
  clearLCD(1);
  lcd.setCursor(9,1); lcd.write((uint8_t) 0);
  lcd.setCursor(9,3); lcd.write((uint8_t) 1);

}

// ================ Fan Menu ================
void fanDisplay(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(17,0); lcd.write((uint8_t) 5);

}

// ================ Reset Menu ================
void resetDisplay(){
  lcd.setCursor(16,0); lcd.write((uint8_t) 4);
  lcd.setCursor(17,0); lcd.write((uint8_t) 5);
}

// ==========================================
void clearLCD(int row) {
  lcd.setCursor(0, row);
  for (int i = 0; i < numCols; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, row);
}

void printTemp(uint8_t temp, int col, int row, int symbol) {
  lcd.setCursor(col, row);
  if(temp < 10) { 
    lcd.print("00");
  }
  else if(temp < 100) { 
    lcd.print("0");
  }
  lcd.print(temp, DEC);
  lcd.print((char) symbol);
}

void printPos(int axis, int row, int col) {
  lcd.setCursor(col, row);
  long steps = read32(axis);
  float decPos = 0;
  switch (axis) {
  case 0: 
    decPos = steps / xSteps; 
    break;
  case 1: 
    decPos = steps / ySteps; 
    break;
  case 2: 
    decPos = steps / zSteps; 
    break;
  }
  // Handle formatting negative numbers
  if (decPos < 0 && decPos > -10) {
    lcd.print(" ");  
  }
  else if (decPos < 10) {
    lcd.print("  ");
  }
  else if (decPos < 100) {
    lcd.print(" ");
  }
  lcd.println(decPos, 2); // was DEC);
  }

float getPos(int axis){
  long steps = read32(axis);
  float decPos = 0;
  switch (axis) {
  case 0: 
    decPos = steps / xSteps; 
    break;
  case 1: 
    decPos = steps / ySteps; 
    break;
  case 2: 
    decPos = steps / zSteps; 
    break;
  }
  return decPos;
}

float calcVersion(uint16_t H, uint16_t L){
  float version;
  uint16_t ver = ((H << 8) + L);
  uint16_t temp1 = ver / 100;
  uint16_t temp2 = ver % 100;
  float temp3 = (float)temp2/10;
  version = temp1 + temp3;
  return version;
}

