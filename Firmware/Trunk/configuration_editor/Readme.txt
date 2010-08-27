Dual Strike Configuration
=========================
You need the firmware 3.0 installed on your Dual Strike. You also need a Java 6
Runtime Environment (for example from Oracle: http://www.java.com/download/)
installed on your PC. Non-Windows users please read "mcc_Readme.txt". It is
assumed that the Java executables are in the path (this is the case most
probably).

Start the configuration editor by double-clicking on 
"configuration_editor_atmega8.bat" if you have a Dual Strike V1 (without 
ATmega168 upgrade) or Dual Strike SMD, use 
"configuration_editor_atmega168.bat" for Dual Strike V2 or a Dual Strike 
V1 upgraded with ATmega168. On non-Windows systems execute the command 
"java -jar mcc-1.1.0.jar configuration_atmega168.xml" for ATmega168 and 
"java -jar mcc-1.1.0.jar configuration_atmega8.xml" for ATmega8 based 
Dual Strikes in the directory this file is residing in. To load and save 
the configuration to the Dual Strike you first have to enter the 
configuration mode by pressing Select on plugging in. By pressing Start 
the configuration mode is left. 


