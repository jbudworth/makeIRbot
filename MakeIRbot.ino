/*-------------------  MakeIRbot v1.8 (ish)  ------------------
Based on original code by codexmas:
https://github.com/codexmas/makeIRbot

This version uses much of the same code but increases the
functionality.
Added:
  * jogging axis x,y,z
  * set extruder and build platform temperature
  * run extruder at settable rpm and duration, fwd and rev
  * toggle fan state
  * reset Makerbot

The code has also been configured for:
  * Arduino-V1.0
  * a 20x4 LCD
  * arduino 328
  * Cupcake with 5D sheild (v3.1 firmware)

Requires additional Libraries:
  * MenuBackend
  * IRremote
  
  
Instructions:
  * Wire up the LCD and IR sensor to the arduino and progam it.
  * When you go to use your remote if the software hasn't already 
    had the IR codes added you will need to do this.  The display 
    will show you any codes it isn't configured to understand.  Add
    these into the IR_Setup.ino file.
  * Be aware that some IR remotes send their control codes more than 
    once. This will prevent things from working correctly. I used an 
    Apple Remote and that worked well. A Sony remote didn't work.
  * Hook up the Digital 0 and Digital 1 up to the Tx and Rx pins on 
    the Makerbots FTDI header.  Also hook up 5V and Gnd.  I used the 
    Gnd on the FTDI header and the 5V from the I2C header, but it 
    doesn't really matter so long as they are connected correctly.
    
Notes:
  * MenuBackend keeps all its menu title strings in RAM, keeping them 
    short is best and then overwrite them as needed.
  * There isn't much RAM left with this code at the moment, if you 
    wanted to add more functionality (eg ethernet) it may be best to 
    move to a Mega.  I tried to shift the menu title strings to PROGMEM 
    but with no success.
    
    
*/



#include <EEPROM.h>
#include <MenuBackend.h>
// *** IRremote Params
#include <IRremote.h>
#include <IRremoteInt.h>
//#include <SoftwareSerial.h>
//#include <avr/pgmspace.h>

const int IR_RECV_PIN = 3;
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// Serial Variables
int serialIndex = 0;
uint8_t serialIn[32];  // for incoming serial data 

//SoftwareSerial Makerbot(A4, A5);

// makeIRbot Variables
const float makeIRbot = 1.83;
uint8_t machineName[16]; // also contains Firmware versions
float MotherboardVersion;
float ExtruderControllerVersion;

unsigned long refreshLast = 0; // Used for tracking refresh intervals
unsigned long refreshInterval = 0; // Set by diferent displays
int currentPhase = 0; // Used by various actions that require multiple steps
int currentPhase1 = 0;
uint8_t commandSent = 0x00;
uint8_t responseRecv = 0x00;
int responseLength = 0;
/* Bit fields for status
 0  :  Connected
 1  :  Building
 2  :  Valid File
 3  :
 4  :
 5  :
 6  :
 7  : Debug Mode
 */
uint8_t flags = 0x00;

const uint8_t extIndex = 0x02;
const uint8_t setextIndex = 0x03;
const uint8_t extTargetIndex = 0x20;
uint8_t extTemp = 0x00;
uint8_t extTarget = 0x00;

const uint8_t hbpIndex = 0x1E;
const uint8_t sethbpIndex = 0x1F;
const uint8_t hbpTargetIndex = 0x21;
uint8_t hbpTemp = 0x00;
uint8_t hbpTarget = 0x00;

uint8_t lastFile[32];

// .s3g file Extension
const uint8_t validExt[4] = {0x2E, 0x73, 0x33, 0x67}; // ".s3g"
// Position data [Axis][Bytes]
uint8_t currentPos[3][4] = {
  {0x00, 0x00, 0x00, 0x00},    // X
  {0x00, 0x00, 0x00, 0x00},    // Y
  {0x00, 0x00, 0x00, 0x00},    // Z
};

/*
 #define xSteps 11.8 // Stock Cupcake settings and pulleys
 #define ySteps 11.8 // Stock Cupcake settings and pulleys
 #define zSteps 320  // Stock Cupcake settings and pulleys
 */

//Cupcake 3G With 5D Shield
// float only has 6-7 decimal digits pricision so cut the value down 
// Numbers approx, see repg cupcake.xml for exact. 
const float xSteps = 94.1397; // per mm
const float ySteps = 94.1397; // per mm
const float zSteps = 2560;    // per mm
const float aSteps = 1600;    // per revolution

const bool CW = true;
const bool CCW = false;
const bool ENABLE = true;
const bool DISABLE = false;
const bool ABSOLUTE = false;
const bool RELATIVE = true;

#include <LiquidCrystal.h>
const int numRows = 4;
const int numCols = 20;
LiquidCrystal lcd(12,11,4,5,6,7);
  
// Keep MenuItem strings short!  These are kept in RAM, 
// too big and the Arduino won't run.
// All MenuItem titles must be unique
MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
MenuItem m_connect =                MenuItem ("01");
MenuItem m_flags =                  MenuItem ("Flags");
MenuItem m_debug =                  MenuItem ("Debug");
MenuItem m_debugtog =               MenuItem ("Debug Toggle");
MenuItem m_temps =                  MenuItem ("Temperature");
MenuItem m_heaters =                MenuItem ("Heaters");
MenuItem m_enable_heaters =         MenuItem ("07");
MenuItem m_disable_heaters =        MenuItem ("08");
MenuItem m_extruder_temp =          MenuItem ("Set EXT Temp");
MenuItem m_extruder_temp_plus =     MenuItem ("10");
MenuItem m_extruder_temp_minus =    MenuItem ("11");
MenuItem m_hbp_temp =               MenuItem ("Set HBP Temp");
MenuItem m_hbp_temp_plus =          MenuItem ("13");
MenuItem m_hbp_temp_minus =         MenuItem ("14");
MenuItem m_file =                   MenuItem ("SD File");
MenuItem m_build =                  MenuItem ("Build File");
MenuItem m_position =               MenuItem ("Position");
MenuItem m_zero =                   MenuItem ("Zero Position");
MenuItem m_jogx =                   MenuItem ("19");
MenuItem m_jogx_plus =              MenuItem ("20");
MenuItem m_jogx_minus =             MenuItem ("21");
MenuItem m_jogy =                   MenuItem ("22");
MenuItem m_jogy_plus =              MenuItem ("23");
MenuItem m_jogy_minus =             MenuItem ("24");
MenuItem m_jogz =                   MenuItem ("25");
MenuItem m_jogz_plus =              MenuItem ("26");
MenuItem m_jogz_minus =             MenuItem ("27");
MenuItem m_extruder =               MenuItem ("Extruder");
MenuItem m_extruder_dir =           MenuItem ("29");
MenuItem m_extruder_rev =           MenuItem ("30");
MenuItem m_extruder_fwd =           MenuItem ("31");
MenuItem m_extruder_rpm =           MenuItem ("32");
MenuItem m_extruder_rpm_plus =      MenuItem ("33");
MenuItem m_extruder_rpm_minus =     MenuItem ("34");
MenuItem m_fan =                    MenuItem ("Toggle Fan");
MenuItem m_reset =                  MenuItem ("Reset");

// EEPROM Index
uint8_t setextTemp = EEPROM.read(0); 
uint8_t sethbpTemp = EEPROM.read(1);
float extruder_RPM = (EEPROM.read(2)/10);

float xpos = 0;
float ypos = 0;
float zpos = 0;

int extruder_duration[] = {1,2,5,10,30,60,300};
int extruder_duration_Index = 3;

//  Distance the build platfom moves each jog
//float jogDistance[] = {0.1, 0.5, 1, 5, 10, 20, 50};
//int jogIndex = 3;
float jog = 5;

bool setMoveDown = false;
bool setMoveUp = false;
bool fanState = false;

byte up_arrow[8] = {0x0,0x0,0x4,0xe,0x1f,0x0,0x0};
byte down_arrow[8] = {0x0,0x0,0x1f,0xe,0x4,0x0,0x0};
byte left_arrow[8] = {0x0,0x2,0x6,0xe,0x6,0x2,0x0};
byte right_arrow[8] = {0x0,0x8,0xc,0xe,0xc,0x8,0x0};
byte updown_arrow[8] = {0x4,0xe,0x1f,0x0,0x1f,0xe,0x4};
byte action[8] = {0x0,0xe,0x11,0x15,0x11,0xe,0x0};

void setup() {
  lcd.createChar(0, up_arrow);
  lcd.createChar(1, down_arrow);
  lcd.createChar(2, left_arrow);
  lcd.createChar(3, right_arrow);
  lcd.createChar(4, updown_arrow);
  lcd.createChar(5, action);
  delay(100);
  lcd.begin(numCols, numRows);
  lcd.clear();
  lcd.print("makeIRbot v");
  lcd.print(makeIRbot);
  delay(250);

  menuSetup();
  menu.moveDown();
  irrecv.enableIRIn(); // Start the receiver
  //Makerbot.begin(38400);
  Serial.begin(38400);
  menu.use(); // Call the connect menu to initialize the connection
}

void loop() {    
  // Watch for Serial Data
  if (Serial.available() > 0) {
    delay(1); // Need delay to properly grab serial data
    serialIn[serialIndex++] = Serial.read();
  }
  else {
    if (serialIndex > 0) {
      if (serialIn[0] == 0xD5) {
        responseRecv = serialIn[2];
        switch(commandSent) {
        case 0x00:  // get Motherboard Version
          switch(currentPhase) {
            case 5: MotherboardVersion = calcVersion(serialIn[4],serialIn[3]); break;
          }
        case 0x04: 
          {// Position query
            int ap = 3; // Start of axis data
            // Loop through each axis
            for (int i = 0; i < 3; i++) {
              // Loop through 4 bytes for each axis
              for (int x = 0; x < 4; x++) {
                currentPos[i][x] = serialIn[ap];
                ap++;
              }
            }
          }
          xpos = getPos(0); 
          ypos = getPos(1);
          zpos = getPos(2);
          break;

        case 0x10: // Print file
          //
          break;
        case 0x0A: // Get Data from tool
          switch (currentPhase) {
          case 1: extTemp   = serialIn[3]; break;
          case 2: extTarget = serialIn[3]; break;
          case 3: hbpTemp   = serialIn[3]; break;
          case 4: hbpTarget = serialIn[3]; break;
          case 6: ExtruderControllerVersion = calcVersion(serialIn[4],serialIn[3]); break;

          }
          break;

        case 0x0C:
          //            if(serialIn[2] == 0x01) {
          bitSet(flags, 0);
          for (int mn = 0; mn < 16; mn++) {
            machineName[mn] = serialIn[mn + 3];
          }
          //  This stopped the controller from connecting
          //            }
          //            else {
          //  bitClear(flags, 0);
          //            }
          break;

        case 0x12: // Read filename from SD Card
          for (int cf = 0; cf < 32; cf++) {
            lastFile[cf] = 0x00;
          }
          for (int rf = 0; rf < 32; rf++) {
            if(serialIn[rf + 4] != 0x00) {
  lastFile[rf] = serialIn[rf + 4];
            }
            else {
  break;
            }
          }
          validateFilename();
          printFilename();
          break;

        default:
          // Pass through
          if(1) {
          }
        }  
        String responseText = "";
        // Deal with response codes
        switch(commandSent) {
          // SD Card response codes
        case 0x10:
        case 0x12:
          switch(serialIn[3]) {
          case 0x00:  /*responseText = "<SUCCESS>";*/
            break;
          case 0x01:  responseText = "<NO CARD>";           break;
          case 0x02:  responseText = "<INIT FAILED>";       break;
          case 0x03:  responseText = "<PARTITION ???>";     break;
          case 0x04:  responseText = "<FS UNKNOWN>";        break;
          case 0x05:  responseText = "<ROOT DIR ???>";      break;
          case 0x06:  responseText = "<CARD LOCKED>";       break;
          case 0x07:  responseText = "<NO SUCH FILE>";      break;
          }
          break;

          // Normal response codes
        default:
          switch(responseRecv) {
          case 0x80:  responseText = "<GENERIC ERROR>";     break;
          case 0x81:  /*responseText = "<OK>";*/            break;
          case 0x82:  responseText = "<BUFF OVERFLOW>";     break;
          case 0x83:  responseText = "<CRC MISMATCH>";      break;
          case 0x84:  responseText = "<QUERY OVERFLOW>";    break;
          case 0x85:  responseText = "<CMD INVALID>";       break;
          case 0x86:  /*responseText = "<SUCCESS>";*/       break;
          case 0x87:  responseText = "<TIMEOUT>";           break;
          default:    responseText = "<CMD UNKNOWN>";
          }
        }
        if (responseText != "") {
          clearLCD(1);
          lcd.print(responseText);
        }
      }
      else {
        // Other serial data
        responseRecv = 0xFF;
        // Assume that we are no longer connected
        bitClear(flags, 0);
      }
      responseLength = serialIndex;
      serialIndex = 0;
    }
    else {
      // Watch for IR Codes
      if (irrecv.decode(&results)) {
        irButtonAction(&results);
        irrecv.resume(); // Receive the next value
      }
      // Refresh various displays
      if (refreshInterval > 0 ) {
        // Trigger refresh
        if (millis() - refreshLast > refreshInterval ) {
          refreshLast = millis();
          menu.use();
        }
      }
    }
  }
  
  if (setMoveDown == true){
    menu.moveDown();
    setMoveDown = false;
  }
  if (setMoveUp == true){
    menu.moveUp(); 
    setMoveUp = false;
  }

  EEPROM.write(0, setextTemp);
  EEPROM.write(1, sethbpTemp);  
  EEPROM.write(2, (extruder_RPM*10));

/*
//  Serial Pass-thru - not tested.
  if(Makerbot.available()){
    Serial.print((uint8_t)Makerbot.read());
  }
  if(Serial.available()){
    Makerbot.print((uint8_t)Serial.read());
  }
*/
}



