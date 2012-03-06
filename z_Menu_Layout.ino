/*


(100)Connect --- (101)Flags --- (102)Debug --- (103)Debug Toggle
  |
  |
  |
  |                       On                 +             +
  |                        |                 |             |
(200)Temperature --- (201)Heaters --- (202)Extruder --- (203)HBP
  |                        |                 |             |
  |                       Off                -             -
  |
  |
  |
(300)File --- (301)Build
  |
  |
  |
  |                                                      +                +                   +
  |                                                      |                |                   |
(400)Position --- (401)Zero --- (402)Jog Unit --- (403)Jog xPos  --- (404)Jog yPos  --- (405)Jog zPos
  |                                                      |                |                   | 
  |                                                      -                -                   -
  |
  |                      Rev              	+
  |                       |                 |
(500)Extruder  ---  (501)Fwd/Back  ---  (502)RPM Set
  |                       |                	|
  |                      Fwd              	-
  |
  |
  |
(600)Fan Toggle
  |
  |
  |
  |
(700)Reset

22 real menu items, 14 momentary items.



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
 