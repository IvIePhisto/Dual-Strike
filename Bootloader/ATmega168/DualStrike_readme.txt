This is the Dual Strike's version of Objective Development's BootloadHID 
bootloader adapted to ATmega168 MCUs. See "Readme.txt" for more details.

"DualStrike_Bootloader_ATmega168.hex" is a firmware image of the bootloader.

Last modification:
06.08.2010

Update of Dual Strike Firmware
==============================

1. While plugging in Dual Strike PCB hold Start.

Expected result : Device is recognized as bootload HID.

2. You can release Start.

3. Start firmware update.

Expected result : Information about updating firmware appears.

4. If the "-r" argument is used for firmware, the Dual Strike will be recognized.
   Otherwise press Home to leave the bootloader.
   
Changes to BootloadHID Bootloader
=================================
-adapted to Dual Strike PCB
-splitted bootloader condition to conditions for entering and leaving
-adapted to ATmega168
-removed commandline tool