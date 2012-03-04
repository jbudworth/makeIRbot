Firstly I need to thank codexmas without whose original work this would not have made it as far as it has. His original code is available on GitHub here: github.com/codexmas/makeIRbot

My code continues to use the majority of his, however I have added some features from his last commit.

This project gives most of the same controls over your Makerbot as ReplicatorG but without the need for a computer.
It will allow you to:
See Makerbot name and firmware versions.
See current temperatures.
Set target temperatures.
Jog X/Y/Z axis.
Run extruder forward and reverse at setable RPM.
Print a file from the SD card.
Enable/Disable the Fan.
Reset the Makerbot.

You will need:
An Arduino. A run of the mill arduino 328 will do just fine with the current code.
A Hitachi 44780 LCD. codexmas' original code was based on a 16x2 display, mine is based on a 20x4.
A 38KHz IR receiver. I just used one that I had in my junk box, codexmas suggests used a TSOP39238CZ1
An IR remote control. I used an Apple Remote as it works well and doesn't have too many more buttons on it than are needed for this project. Some remotes will not work as they send their codes multiple times, the Sony remote I tried did this.

The code uses additional libraries to do the heavy lifting for the menu, specifically MenuBackend and IRremote for the remote handling. I have attached the specific versions I used.

To Build:
Wire up the display and the arduino as shown in the image and program the arduino.
The code was intended to be used with the Arduino-1.0 IDE and will almost certainly have issues with anything earlier. The additional libraries (I think) have been updated to work with 1.0 as well.

Hook up the arduino's serial pins to pins 4 and 5 on the Makerbot's FTDI header. For testing I hooked up the arduino's Gnd to the Black (pin 1) on the FTDI header and +5V to the I2C header.
If you have wired everything correctly the first screen should tell you the name of your Makerbot and the firmware versions of the motherboard and extruder controller.
Then you will need to add your IR remote controls codes into the arduino sketch, press the buttons on your remote that you want to use and note the numbers that come up on the LCD. You will need to add these into the IR_Setup.ino file in the appropriate places and then reprogram the arduino again.

I coded this to work with a Cupcake Makerbot with a 5D shield and version 3.1 firmware. However I think it may work on the Thing-o-matic as well. The jog function will likely have to be redone to work with anything older I suspect.


-----------------------
     makeIRbot 1.01
-----------------------
By Gord Christmas <codexmas@gmail.com> April 25/2011
Hanging out on #makerbot as codeXmas


Purpose?
-----------------------
1. To be able to use a Makerbot without a computer attached, or having to start
    a print from the SD card via ReplicatorG.
2. Control a Makerbot externally by simulating the serial protocol that
    ReplicatorG implements.
3. Provide as much functionality as ReplicatorG control panel without hacking
    the firmware for the machine.
4. Learn C/C++ and hack things with Arduino


What do you need?
-----------------------
1. An Arduino of any form that is an ATMega328 or greater.
    <http://www.solarbotics.com/products/kardw/> $10 Super compact version.
2. A 3 pin 38kHz IR Receiver module. I used <TSOP39238CZ1>
3. A 16x2 LCD that is HD44780 compatible. Will also need a 10k pot for contrast
    control.
4. <optional> Piezo buzzer for future audio feedback.
5. Libraries Used
    a) MenuBackend <http://www.arduino.cc/playground/uploads/Profiles/MenuBackend_1-4.zip>
    b) IRremote <https://github.com/shirriff/Arduino-IRremote/>
    c) LiquidCrystal <Built-in Arduino Library>

I used the LED on Pin 13 as a status light, part of the Ardweeny kit, and most
every Arduino out there


How do I make it work?
-----------------------
First off, breadboard the parts together using the pinouts that are specified in
the Sketch. You will need a +5v source, I used an extra floppy drive connector
from the bot. Pins on it are Red [+5v] Black [GND] Black [GND] Yellow [+12v]

Then attach jumper wires from Pin 0 and Pin 1 for the Rx/Tx lines on the Arduino to
the Makerbot FTDI header pins 4&5.

They might be reversed, so if you are getting no response at all, swap the Rx/Tx
wires and try again.

The Debug menu will show the raw data that has been received in the last packet.
It also shows the command that was sent, the command code received, the payload
size, as well as the CRC on the first line.