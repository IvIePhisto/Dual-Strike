                              Trackball Strike
                                       
                   Copyright 2010 Jochen Zurborg and Michael Pohl
                                       
                           http://www.zurborg.info

All code, schematics, firmware and other files included within this
archive are licensed under the GNU Public License V3, see license.txt for more
details.

Introduction
============
The Dual Strike is a project of a easy-to-use controller PCB. 
Trackball Strike is an alternate firmware implementing a three-button mouse. It
requires a rotary encoder device like a trackball, a spinner or a mouse.

Contents
========
The files within this archive are the source code for the Dual Strike and its SMD
design as an AVR Studio project.

Setup
=====
Connect as follows:
-VCC:   5 Volt
-GND:   GROUND
-UP:    X1
-DOWN:  X2
-LEFT:  Y1
-RIGHT: Y2
-LP:    Button 1 Signal
-LK:    Button 2 Signal
-MK:    Button 3 Signal

X1/X2 and Y1/Y2 are supposed to be rotary encoders.
The buttons have to be common ground switches (i.e. switching against ground).

Startup Behavior
================
To update the firmware connect START to GND (e.g. by pressing a button) while
plugging in to switch to firmware update mode. For ATmega168 based Dual Strikes
(like V2) you can release START after plugging in, for the ATmega8 based Dual
Strikes (like V1) hold it while updating.
Execute the appropiate batch file:
-ATmega168 (V2): update_firmware_atmega168.bat
-ATmega168 SMD:  update_firmware_atmega168_smd.bat
-ATmega8 (V1):   update_firmware_atmega8.bat
-ATmega8 SMD:    update_firmware_atmega8_smd.bat

Connect HOME to GND (e.g. by pressing a button) while plugging in to switch to
pass-through mode.