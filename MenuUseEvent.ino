void menuUseEvent(MenuUseEvent used){
  if (used.item == m_connect) {
    currentPhase++;
        switch (currentPhase) {
    case 1: 
    // Only query machine if we are not connected already
      if (!bitRead(flags, 0)) {
        queryMachineName();
      }
      break;
    case 2: 
    case 3: 
    case 4: 
    case 5: if (MotherboardVersion == NULL){ getVersion(); break;}
    case 6: if (ExtruderControllerVersion == NULL){ getToolVersion(); break;}
      default: currentPhase = 0; 
      infoDisplay();
    }
  }  
  else if (used.item == m_flags) {
    flagDisplay();
  }
  else if (used.item == m_debug) {
    debugDisplay();
  }
  else if (used.item == m_debugtog) {
    // Toggle the debug mode on or off
    if (bitRead(flags, 7)) {
      bitClear(flags, 7);
    }
    else {
      bitSet(flags, 7);
    }
    menu.moveLeft();
  }
  else if (used.item == m_temps) {
    currentPhase++;
    switch (currentPhase) {
    case 1: 
      getTemp(extIndex);       
      break;
    case 2: 
      getTemp(extTargetIndex);       
      break;
    case 3: 
      getTemp(hbpIndex);       
      break;
    case 4: 
      getTemp(hbpTargetIndex);       
      break;
    default: 
      currentPhase = 0; 
      tempDisplay(); // Reset phase then update display
    }
  }
  else if (used.item == m_extruder_temp) {
    // Set Extruder Temperature - To be Implimented
    setTemp(setextIndex, setextTemp);
  }
  else if (used.item == m_hbp_temp) {
    // Set Heated Build Platform Temperature - To be Implimented
    setTemp(sethbpIndex, sethbpTemp);
  }
  else if (used.item == m_file) {
    if (lastFile[0] == 0x00) { // Initial setting, or last entry on SD card
      fetchFirstFilename();
    }
    else {
      fetchNextFilename();
    }
    delay(50);
    printFilename();
  }
  else if (used.item == m_build) {
    if (bitRead(flags, 2)) {
      playbackFile(lastFile);
      menu.moveLeft();
    }
    else {
      clearLCD(1);
      lcd.print("<Invalid>");
    }
  }
  else if (used.item == m_position) {
    currentPhase++;
    switch (currentPhase) {
    case 1: 
      getPosition();       
      break;
    default: 
      currentPhase = 0; 
      positionDisplay();
    }
  }
  else if (used.item == m_zero) {
    setPosition(0,0,0);
    menu.moveLeft();
  }
  else if (used.item == m_jogx) {
    currentPhase++;
    switch (currentPhase) {
    case 1: 
      getPosition();       
      break;
    default: 
      currentPhase = 0; 
      jogxDisplay();
    }
  }
  else if (used.item == m_jogy) {
    currentPhase++;
    switch (currentPhase) {
    case 1: 
      getPosition();       
      break;
    default: 
      currentPhase = 0; 
      jogyDisplay();
    }
  }
  else if (used.item == m_jogz) {
    currentPhase++;
    switch (currentPhase) {
    case 1: 
      getPosition();       
      break;
    default: 
      currentPhase = 0; 
      jogzDisplay();
    }
  }
  else if (used.item == m_extruder_dir) {
    // 1,2,5,10,30,60,300 seconds
    if (extruder_duration_Index == 6){
      extruder_duration_Index = 0;}
    else {extruder_duration_Index++;}
    extruder_dir();
  }
  else if (used.item == m_fan) {
	fanState = !fanState;
	if (fanState == true){
		enableFan();
	}
	if (fanState == false){
		disableFan();
	}
}
  else if (used.item == m_reset) {
    reset();
    menu.moveDown();

}
}



