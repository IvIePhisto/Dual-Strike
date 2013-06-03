This is the Dual Strike's version of Objective Development's BootloadHID 
bootloader adapted to ATmega8 MCUs. See "Readme.txt" for more details.

Last modification:
06.08.2010

Update of Dual Strike Firmware
==============================

1. While plugging in Dual Strike PCB hold Start and don't release it.

Expected result : Device is recognized as bootload HID.

3. Start firmware update.

Expected result : Information about updating firmware appears.

4. Release start to quit firmware update mode. The Dual Strike should
   be recognized.
   
Changes to BootloadHID Bootloader
=================================
-adapted to Dual Strike PCB
-removed commandline tool