/*


Connect      -----  Flags     -----  Debug  -----  Debug Toggle
  |
  |
  |
  |                   On                 +             +
  |                    |                 |             |
Temperature  -----  Heaters   -----  Extruder  -----  HBP
  |                    |                 |             |
  |                   Off                -             -
  |
  |
  |
File         -----  Build
  |
  |
  |
  |                                  +            +            +
  |                                  |            |            |
Position     -----  Zero  -----  Jog xPos  --- Jog yPos  --- Jog zPos
  |                                  |            |            | 
  |                                  -            -            -
  |
  |                    Rev           	+
  |                     |             	|
Extruder     -----  Fwd/Back  -----  RPM Set
  |                     |             	|
  |                    Fwd          	-
  |
  |
  |
Fan Toggle
  |
  |
  |
  |
Reset
*/



/*  

To do:
	* Add serial passthrough (possible?)
		- Software.serial (38400)
 		- Which pins? (A4, A5)
 		- Which signals have priority?

	* Add keypad to control
    * Settable jog distance

Done:
	* Position Jog
	* Fan control
	* Extruder Control
 		- Fwd
 		- Rev
 		- RPM
  */
 