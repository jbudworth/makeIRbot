
void menuChangeEvent(MenuChangeEvent changed) {
  clearLCD(3);
  clearLCD(2);
  clearLCD(1);
  clearLCD(0);
  lcd.print(changed.to.getName());
  refreshInterval = 0; // Always clear refresh interval when the menu changes
  currentPhase = 0; // Always set the current phase back to zero, as not all events have phases

  //=======================  Connect  =======================  
  if (changed.to.getName() == m_connect) {
    refreshInterval = 200;
    infoDisplay();
  }
  else if (changed.to.getName() == m_debug) {
    refreshInterval = 500;
    debugDisplay();
  }
  else if (changed.to.getName() == m_flags) {
    refreshInterval = 100;
    flagDisplay();
  }
  else if (changed.to.getName() == m_debugtog) {
    debugtogDisplay();
  }

  //=======================  Temperature  =======================  
  else if (changed.to.getName() == m_temps) {
    refreshInterval = 250;
    tempDisplay();
  }
  else if (changed.to.getName() == m_heaters){
    heatersDisplay();
  }
  else if (changed.to.getName() == m_enable_heaters){
    setTemp(setextIndex, setextTemp);
    delay(50);
    setTemp(sethbpIndex, sethbpTemp);
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_disable_heaters){
    setTemp(setextIndex, 0);
    delay(50);
    setTemp(sethbpIndex, 0);
    setMoveUp = true;
  }
  else if (changed.to.getName() == m_extruder_temp) {
    extruder_temp();
  }
  else if (changed.to.getName() == m_extruder_temp_plus) {
    setextTemp++;
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_extruder_temp_minus) {
    setextTemp--;
    setMoveUp = true;
  }
  else if (changed.to.getName() == m_hbp_temp) {
    hbp_temp();
  }
  else if (changed.to.getName() == m_hbp_temp_plus) {
    sethbpTemp++;
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_hbp_temp_minus) {
    sethbpTemp--;
    setMoveUp = true;
  }

  //===========================  File  ===========================    
  else if (changed.to.getName() == m_file) {
    fileSelect();
  }
  else if (changed.to.getName() == m_build) {
    fileBuild();
  }

  //=======================  Position  =======================    
  else if (changed.to.getName() == m_position) {
    refreshInterval = 250;
    positionDisplay();
  }
  else if (changed.to.getName() == m_zero) {
    zeroposDisplay();
  }
  else if (changed.to.getName() == m_jog_unit) {
    jogunitDisplay();
  }
  else if (changed.to.getName() == m_jogx){
    refreshInterval = 250;
    jogxDisplay();
  }
  else if (changed.to.getName() == m_jogx_plus){
    queueXPointNS2(jogDistance[jogIndex],0,0, 500);
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_jogx_minus){
    queueXPointNS2(-jogDistance[jogIndex],0,0, 500); 
    setMoveUp = true;
  }
  else if (changed.to.getName() == m_jogy){
    refreshInterval = 250;
    jogyDisplay();
  }
  else if (changed.to.getName() == m_jogy_plus){
    queueXPointNS2(0,jogDistance[jogIndex],0, 500);
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_jogy_minus){
    queueXPointNS2(0,-jogDistance[jogIndex],0, 500);
    setMoveUp = true;
  }
  else if (changed.to.getName() == m_jogz){
    refreshInterval = 250;
    jogzDisplay();
  }
  else if (changed.to.getName() == m_jogz_plus){
    queueXPointNS2(0,0,jogDistance[jogIndex], 5500);
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_jogz_minus){
    queueXPointNS2(0,0,-jogDistance[jogIndex], 5500);
    setMoveUp = true;
  }

  //=======================  Extruder  =======================    
  else if (changed.to.getName() == m_extruder) {
    extruder();
  }
  else if (changed.to.getName() == m_extruder_dir) {
    extendedStop();
    extruder_dir();
  }
  else if (changed.to.getName() == m_extruder_rev){
  queueXPointNS(extruder_RPM, extruder_duration[extruder_duration_Index], CCW);
  extruder_dir_rev();
  }
  else if (changed.to.getName() == m_extruder_fwd){
  queueXPointNS(extruder_RPM, extruder_duration[extruder_duration_Index], CW);
  extruder_dir_fwd();
  }
  else if (changed.to.getName() == m_extruder_rpm){
    extruder_rpm();
  }
  else if (changed.to.getName() == m_extruder_rpm_plus){
    extruder_RPM = (extruder_RPM+0.1);
    setMoveDown = true;
  }
  else if (changed.to.getName() == m_extruder_rpm_minus){
    extruder_RPM = (extruder_RPM-0.1);
    setMoveUp = true;
  }

  //=======================  Fan  =======================    
  else if (changed.to.getName() ==  m_fan){
    fanDisplay();
  }

  //=======================  Reset  =======================    
  else if (changed.to.getName() ==  m_reset){
    resetDisplay();
  }




}


