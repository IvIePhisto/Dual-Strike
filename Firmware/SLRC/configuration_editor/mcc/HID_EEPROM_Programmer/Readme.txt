HID EEPROM Programmer
=====================

Author: Michael Pohl

This is tool allows programming and dumping of EEPROM data of MCUs 
implementing a special HID device. It is based on Objective 
Development's bootloadHID program. 
This program stands under the GNU General Public License V3.
See "Licence.txt" for original licensing details from Objective Development.
See "example.c" to get to know the MCU behavior required.

Execute "make" ("gcc" has to be installed) to create a non-Windows executable.
Use Cygwin's "make" on Windows together with the "Makefile.windows" as the file
argument: make -f Makefile.windows


Differences to bootloadHID
~~~~~~~~~~~~~~~~~~~~~~~~~~
-added EEPROM programming and dumping capabilities
-removed flash programming and rebooting
-added different return codes depending on the execution outcome - see "return_codes.txt" for a list of them.


Return Codes
~~~~~~~~~~~~
0: Completed Successfully
1: Wrong Arguments
2: Open Device Error
3: Open EEPROM File Error
4: Empty EEPROM File
5: EEPROM Programming Error
6: EEPROM Dumping Error
7: Writing EEPROM Dump File Error
