This is the Dual Strike's version of Objective Development's BootloadHID 
commandline tool. See "Readme.txt" for more details.

Last modification:
06.08.2010

Update of Dual Strike Firmware
==============================

1. While plugging in Dual Strike PCB hold Start.

Expected result : Device is recognized as bootload HID.

2. You can release Start if your Dual Strike uses a ATmega168 MCU.

3. Start firmware update.

Expected result : Information about updating firmware appears.

4. On Dual Strikes with ATmega168 MCU if the "-r" argument is used for
   firmware, the Dual Strike will be recognized. Otherwise press Home to leave
   the bootloader. On Dual Strikes with ATmega8 MCU release Start.
   
Changes to BootloadHID
======================
-added test mode (no arguments), more verbose output
-added compatibility with the RetroAdapter Bootloader