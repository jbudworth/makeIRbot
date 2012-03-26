// For all details on the Makerbot serial protocol see the
// Reprap Generation 3 Protocol Specification v1 document
// http://docs.google.com/View?docid=dgzjfrcc_2gm84ggdb
/*  Not used
void queryMakerbotInfo() { // Fetch build name of machine (Cupcake) firmware build name
  uint8_t data[]= {0x14, 0x18, 0x00};
  sendBytesWithCRC(data, sizeof(data));
}
*/
void getVersion(){  //  Mimic version 29 of RepG while asking for Mobo firmware version
  uint8_t data[] = {0x00, 0x1D, 0x00};
  sendBytesWithCRC(data, sizeof(data));   
}

void initMakerbot(){
  uint8_t data[] = {0x01};
  sendBytesWithCRC(data, sizeof(data));
}

void getToolVersion(){
  uint8_t data[] = {0x0A,0x00,0x00};
  sendBytesWithCRC(data, sizeof(data));
}

void queryMachineName() { // Read 16 chars from EEPROM at Offset 32  
  uint8_t data[] = {0x0C, 0x20, 0x00, 0x10};
  sendBytesWithCRC(data, sizeof(data));
}

void fetchFirstFilename() {
  uint8_t data[]= {0x12, 0x01};
  sendBytesWithCRC(data, sizeof(data));
}

void fetchNextFilename() {
  uint8_t data[]= {0x12, 0x00};
  sendBytesWithCRC(data, sizeof(data));
}

void motor1DIR(bool dir){
  uint8_t data[]= {0x0A, 0x00, 0x08, 0x00};
  if (dir == true){
    data[3] = 0x01;
  }
  else { data[3] = 0x00;}
  sendBytesWithCRC(data, sizeof(data));
}

void motor1EN(bool en){
  uint8_t data[]= {0x0A, 0x00, 0x0A, 0x00};
  if (en == true){
    data[3] = 0x01;
  }
  else { data[3] = 0x00;}
  sendBytesWithCRC(data, sizeof(data));
}

void motor1RPM(float rpm){
  uint8_t data[] = {0x0A,0x00,0x06,0x00,0x00,0x00,0x00};
  uint32_t temp = (rpm * 60 * 1000000);
  data[3] = (temp & 0x000000FF);
  data[4] = (temp & 0x0000FF00)>>8;
  data[5] = (temp & 0x00FF0000)>>16;
  data[6] = (temp & 0xFF000000)>>24;
  sendBytesWithCRC(data, 7);
}

void motor1PWM(uint8_t PWM){
uint8_t data[] = {0x0A,0x00,0x04,0x00};
data[3] = PWM;
  sendBytesWithCRC(data, sizeof(data));
}

void motor1_DDA(float RPM, int runTime){
  uint8_t data[] = {0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint32_t temp = (1000000/(aSteps/60)) / RPM;  // uSecs between steps
  data[1] = (temp & 0x000000FF);
  data[2] = (temp & 0x0000FF00)>>8;
  data[3] = (temp & 0x00FF0000)>>16;
  data[4] = (temp & 0xFF000000)>>24;
  data[5] = data[1]; 
  data[6] = data[2];
  data[7] = data[3];
  data[8] = data[4];
  uint32_t steps = (aSteps/60) * RPM * runTime; //Number of steps to take
  data[9] =  (steps & 0x000000FF);
  data[10] = (steps & 0x0000FF00)>>8;
  data[11] = (steps & 0x00FF0000)>>16;
  data[12] = (steps & 0xFF000000)>>24;
  sendBytesWithCRC(data, 13);
}

void extDirEn(bool dir, bool enable){
//  0 = false,false = CCW/Off
//  1 = false,true = CCW/On
//  2 = true,false = CW/Off
//  3 = true,true = CW/On
  uint8_t data[]= {0x0A, 0x00, 0x0A, 0x00};
  if (dir == true){data[3] = 0b00000010;} 
  if (enable == true){data[3] = data[3] | 0b00000001;}
  sendBytesWithCRC(data, sizeof(data));
}

void setTemp(uint8_t toolIndex, uint8_t tooltemp) {
  uint8_t data[]= {    0x0A, 0x00, toolIndex, tooltemp    };
  sendBytesWithCRC(data, sizeof(data));
}

void getTemp(uint8_t toolIndex) {
  uint8_t data[]= {    0x0A, 0x00, toolIndex    };
  sendBytesWithCRC(data, sizeof(data));
}

void getPosition() {
  uint8_t data[] = {0x04};
    sendBytesWithCRC(data, 1);
}

void setPosition(float x, float y, float z) {
  uint8_t data[] = {0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  long temp = (x * xSteps);
  data[4] = (temp & 0xFF000000)>>24;
  data[3] = (temp & 0x00FF0000)>>16;
  data[2] = (temp & 0x0000FF00)>>8;
  data[1] = (temp & 0x000000FF);
  temp = (y * ySteps);
  data[8] = (temp & 0xFF000000)>>24;
  data[7] = (temp & 0x00FF0000)>>16;
  data[6] = (temp & 0x0000FF00)>>8;
  data[5] = (temp & 0x000000FF);
  temp = (z * zSteps);
  data[12] = (temp & 0xFF000000)>>24;
  data[11] = (temp & 0x00FF0000)>>16;
  data[10] = (temp & 0x0000FF00)>>8;
  data[9] = (temp & 0x000000FF);
  sendBytesWithCRC(data, 13);
}

// Move to absolute point
void queuePoint(float x, float y, float z) {
  uint8_t data[] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  long temp = (x * xSteps);
  data[1] = (temp & 0x000000FF);
  data[2] = (temp & 0x0000FF00)>>8;
  data[3] = (temp & 0x00FF0000)>>16;
  data[4] = (temp & 0xFF000000)>>24;
  temp = (y * ySteps);
  data[5] = (temp & 0x000000FF);
  data[6] = (temp & 0x0000FF00)>>8;
  data[7] = (temp & 0x00FF0000)>>16;
  data[8] = (temp & 0xFF000000)>>24;
  temp = (z * zSteps);
  data[9] =  (temp & 0x000000FF);
  data[10] = (temp & 0x0000FF00)>>8;
  data[11] = (temp & 0x00FF0000)>>16;
  data[12] = (temp & 0xFF000000)>>24;
  // Delta = 250 uS per step adjust as needed
  data[13] = 0xFA;
  data[14] = 0x00;
  data[15] = 0x00;
  data[16] = 0x00;
  sendBytesWithCRC(data, 17);
  delay(50);
  disableSteppers();
}

void getExtendedPosition(){
  uint8_t data[] = {0x15};
    sendBytesWithCRC(data, 1);
}

// For X Y Z Jogging
void queueXPointNS2(float x, float y, float z, long duration){ // Duration in ms
  uint8_t data[] = {0x8E, 
  0x00, 0x00, 0x00, 0x00,   //X
  0x00, 0x00, 0x00, 0x00,   //Y
  0x00, 0x00, 0x00, 0x00,   //Z
  0x00, 0x00, 0x00, 0x00,   //A
  0x00, 0x00, 0x00, 0x00,   //B
  0x00, 0x00, 0x00, 0x00,    //duration
  0x1F};                     // Relative or absolute
  long temp = (x * xSteps);
  data[1] = (temp & 0x000000FF);
  data[2] = (temp & 0x0000FF00)>>8;
  data[3] = (temp & 0x00FF0000)>>16;
  data[4] = (temp & 0xFF000000)>>24;
  temp = (y * ySteps);
  data[5] = (temp & 0x000000FF);
  data[6] = (temp & 0x0000FF00)>>8;
  data[7] = (temp & 0x00FF0000)>>16;
  data[8] = (temp & 0xFF000000)>>24;
  temp = (z * zSteps);
  data[9] =  (temp & 0x000000FF);
  data[10] = (temp & 0x0000FF00)>>8;
  data[11] = (temp & 0x00FF0000)>>16;
  data[12] = (temp & 0xFF000000)>>24;
  temp = duration * 1000;
  data[21] = (temp & 0x000000FF);
  data[22] = (temp & 0x0000FF00)>>8;
  data[23] = (temp & 0x00FF0000)>>16;
  data[24] = (temp & 0xFF000000)>>24;
  sendBytesWithCRC(data,sizeof(data)); //26
  delay(200);
  disableSteppers();
}

// For extruder moves
void queueXPointNS(float rpm, uint32_t duration, bool dir){ // Used for Extruder Control
  uint8_t data0[] = {0x88, 0x00, 0x0A, 0x01, 0x03};
  sendBytesWithCRC(data0, 5);
  uint8_t data1[] = {0x88, 0x00, 0x04, 0x01, 0xFF};
  sendBytesWithCRC(data1, 5);
  uint8_t data[] = {0x8E, 
  0x00, 0x00, 0x00, 0x00,   //X
  0x00, 0x00, 0x00, 0x00,   //Y
  0x00, 0x00, 0x00, 0x00,   //Z
  0x00, 0x00, 0x00, 0x00,   //A
  0x00, 0x00, 0x00, 0x00,   //B
  0x00, 0x00, 0x00, 0x00,    //duration
  0x1F};                     // Relative or absolute
int32_t temp = (((aSteps * rpm)/60) * duration);
  if (dir == CW){temp = temp * -1;}
  data[13] = (temp & 0x000000FF);
  data[14] = (temp & 0x0000FF00)>>8;
  data[15] = (temp & 0x00FF0000)>>16;
  data[16] = (temp & 0xFF000000)>>24;
  uint32_t temp1 = duration * 1000000;
  data[21] = (temp1 & 0x000000FF);
  data[22] = (temp1 & 0x0000FF00)>>8;
  data[23] = (temp1 & 0x00FF0000)>>16;
  data[24] = (temp1 & 0xFF000000)>>24;
  sendBytesWithCRC(data,sizeof(data)); //26
  delay(200);
  disableSteppers();
}

void extendedStop(){
    uint8_t data[] = {0x16,0x03};
    sendBytesWithCRC(data, sizeof(data));
}

void reset(){
    uint8_t data[] = {0x11};
    sendBytesWithCRC(data, sizeof(data));
}

void disableSteppers(){
  uint8_t data[] = {0x89, 0b00001111};
  sendBytesWithCRC(data, 2);
}

void enableFan() {
  uint8_t data[] = {0x0A,0x00,0x0C,0x01};
  sendBytesWithCRC(data,  sizeof(data));
}

void disableFan() {
  uint8_t data[] = {0x0A,0x00,0x0C,0x00};
  sendBytesWithCRC(data, sizeof(data));
}

void coolFanON(bool state) {
  if (state == true){
    uint8_t data[] = {0x0A,0x00,0x0C,0x01};
    sendBytesWithCRC(data, sizeof(data));
  }
  if (state == false){
    uint8_t data[] = {0x0A,0x00,0x0C,0x00};
    sendBytesWithCRC(data, sizeof(data));
  }
}

void sendBytesWithCRC(uint8_t *data, int len){
  uint8_t crc = 0x00;
  int length = len;
  int a;
  int fn = 1;
  commandSent = data[0];
  Serial.write(0xD5);    // Send header Identifier byte
  Serial.write(length);  // Send payload length
  // Send Payload
  for(int p = 0; p < length; p++){
    Serial.write((uint8_t)data[p]);
  }
  // Send calculated CRC
  for(a = 0; a < length; a++) {
    //delay(1);
    crc = calculateCRC(crc, data[a]);
  }
  Serial.write((uint8_t)crc);
}

uint8_t calculateCRC (uint8_t crc, uint8_t data) {
  uint8_t cc;
  crc = crc ^ data;
  for (cc = 0; cc < 8; cc++) {
    if (crc & 0x01)
      crc = (crc >> 1) ^ 0x8C;
    else
      crc >>= 1;
  }
  return crc;
}

uint32_t read32(int axis) {
  union {
    // AVR is little-endian
    int32_t a;
    struct {
      uint8_t data[4];
    } 
    b;
  } 
  shared;
  shared.b.data[0] = currentPos[axis][0];
  shared.b.data[1] = currentPos[axis][1];
  shared.b.data[2] = currentPos[axis][2];
  shared.b.data[3] = currentPos[axis][3];
  return shared.a;
}



