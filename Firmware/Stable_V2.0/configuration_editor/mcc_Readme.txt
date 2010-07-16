Microcontroller Configuration Framework
=======================================

Author:    Michael Pohl
 Email:    pohl-michael@gmx.biz
 Homepage: http://pohl-michael.gmxhome.de

License: GNU General Public License V3
  (see "gpl-3.0.txt" or http://www.gnu.org/licenses/gpl-3.0.html)
Some files were released under different licenses, see the appropiate text
files for more information.

This is a framework for bit-based configurations giving the end user a
comfortable GUI and the developer easy handling of persistent settings.
It is aimed at usage with AVR microcontrollers to read out, modify and update
EEPROM configuration data. It makes heavy use of the XML technologies XML 
Schema and XSL Transformations.

The PC software part requires a Java 6 Runtime Environment. On all systems
except Windows custom compilation of the "HID Programmer" is necessary. See
"HID_Programmer/Readme.txt" for more information on custom compilation.


Usage
~~~~~
You have to install a Java 6 Runtime Environment or newer (for example from
Oracle: http://www.java.com/download/).
As an end user you should have gotten a package containing all necessary files,
see "Readme.txt" for more information. Then start the "mcc-*.*.*.jar" file
either by double clicking on it (for example on Windows) or calling
"javaw -jar mcc-*.*.*.jar" (exchange * by the appropiate version number) in
the directory this file resides in.

Version History
~~~~~~~~~~~~~~~
1.0.1 - fixed choice bits reading bug
