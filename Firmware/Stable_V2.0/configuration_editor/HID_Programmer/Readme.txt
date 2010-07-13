HID Programmer
==============

Author: Michael Pohl

This is an extented version of Objective Development's bootloadHID program.
This program stands under the GNU General Public License V3.
See "Licence.txt" for original licensing details from Objective Development.

Execute "make" ("gcc" has to be installed) to create a non-Windows executable.
Use Cygwin's "make" on Windows together with the "Makefile.windows" as the file
argument: make -f Makefile.windows

Differences to bootloadHID
~~~~~~~~~~~~~~~~~~~~~~~~~~
Addition of EEPROM programming capabilities.
Different return codes (except 0 and 1) depending on the processing outcome.
See "return_codes.txt" for a list of them.
Also works for devices with the Retro Adapter's <http://retro.world3.net/> 
bootloader Vendor and Product information.