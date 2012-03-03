void irButtonAction(decode_results *results) {
  int code = (int) results->value;
  if(code != -1) {
    switch(code) {
      // Set to use Apple Remote control
      case 20515: menu.moveUp(); break;    // Up Arrow
      case -12051: menu.moveUp(); break;    // Up Arrow
            
      case 12323: menu.moveDown(); break;   // Down Arrow
      case -20243: menu.moveDown(); break;   // Down Arrow
      
      case -28637: menu.moveLeft(); break;   // Left Arrow
      case 4333: menu.moveLeft(); break;   // Left Arrow
            
      case 24611: menu.moveRight(); break;  // Right Arrow
      case -7955: menu.moveRight(); break;  // Right Arrow
            
      case -24541: menu.use(); break;       // OK/Select Arrow
      case 8429: menu.use(); break;       // OK/Select Arrow
      
      default:
        clearLCD(1);
        lcd.print("?IR:            ");
        lcd.setCursor(4, 1);
        lcd.print(code);
    }
  }
}
