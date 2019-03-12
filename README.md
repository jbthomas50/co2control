# co2control
**co2 control system**
 
This system controls a solenoid attached to a co2 tank. There is a sensor in the system that tells the arduino the concentration of co2 in the system, and then the solenoid can open and close to get thee correct concentration of co2. The concentration of co2 is set by the user using two potentiometers. 
There will also be fans within the system to create circulation. 
There is an LCD screen to display the co2 concentration setting and the current concentration.

*Suggestions*

The LiquidCrystal.h library for the arduino isn't good. Go to 
the following website to download a better version, rename it, and replace 
it with the current arduino LiguidCrystal.h library.
https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/?tab=downloads

To use the ESP 8266 wifi module, follow the instructions located 
at this website:

