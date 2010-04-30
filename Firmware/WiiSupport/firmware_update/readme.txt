                             Dual Strike v1.0
                                       
                         Copyright 2009 Jochen Zurborg
                                       
                           http://www.zurborg.info

The firmwares and other files included within this  archive are licensed under
the GNU Public License V3, see license.txt for more details.

History
=======
1.7.0  -major changes in source
       -added Wii Classic Controller working mode (EXPERIMENTAL)

1.6.2  -merged SMD design code into source code

1.6.1  -bug solved: triggers not working after being used to switch to
        non-default mode

1.6    -changed configuration mode behaviour again
       -re-introduced inverted trigger configurablity
       -added joystick mode switch support
       -added joystick mode switch emulation
       -Dual Stick USB polling interval is now 1ms
       
1.5    -changed configuration mode behaviour
       -added possibility to reset to the defaults

1.4    - added right stick support
       - removed configuration possibility of inverted triggers
       
1.3    - added configuration capabilities
       - PS3 Home Button support
       - inverted trigger support removed from build, still usable through 
         recompilation

1.2    - HID Device extended
       - Inverted Trigger Support
        
1.1    - HID Device extended 
        
1.0    - initial release


Introduction
============
The Dual Strike PCB is a completely solderless board for your arcade stick.
Every connection to buttons or directions screw in directly to the side of the
board, so installation requires nothing more than a screwdriver. Dual Strike
works on any PC (including Mac, Windows 7/Vista/XP/2K/98, and Linux) as well as
Playstation 3 systems.

This makes the dual strike the perfect solution for customers who want to build 
an arcade stick.


Features 
========
    * 20 screw terminal ports for connecting all of your wires
    * No soldering required
    * USB ‘B’ jack for connecting any standard USB cable
    * Piggyback interface for easy installation of additional xbox360 pads
    * Integrated switch interface for easy configuration between xbox360 and PS3
    * Easy to use dual mod interface supporting dreamcast or xbox360
    * Seperate connection points for all power lines and signals
    * Mounting holes for easy installation inside your arcade board
    * Works on PC, PS3 and MAC out of the box
    * Compatible with all operating systems that understand the HID protocol and
      support gamepad devices (including Mac, Windows™ 7/Vista/XP/2000/98 &
      Linux)
    * Compatible with Playstation 3
    * Firmware update is supported for Windows systems
    * By connecting the pins S3 and S4 and configuring the Dual Strike one of 
      the following is supported:
        - joystick mode switch (like on MadCatz Fightsticks and Fightpads)
        - emulation of a joystick mode switch for the pass-through device (like
          a MadCatz Fightstick and Fightpad PCB)
        - pass-through PCBs with inverted (active low) triggers


Installation
============
Connect all wires/pins to the buttons and the joystick and connect the USB lines
to the PCB to pass through (e.g. by soldering).

For the pins S3 and S4 you have four options:
    * let them unconnected
    * connect them to the pins of a joystick mode switch (S3=left stick,
      S3=right stick) to read the setting
    * connect them to the pins on the pass-through PCB of a joystick mode
      switch (S3=left stick, S4=right stick) to emulate the switch (if the left 
      or right joystick is enabled together with the digital pad, for the
      pass-through device the digitial pad will not be activated)
    * connect them to the trigger pins on a pass-through PCB with active low
      triggers (S3=left trigger, S4=right trigger)
See Extra Pins Mode in the Configuration Mode section for how to configure the
Dual Strike accordingly.


Startup Behaviour
=================
If a button or joystick direction is pressed, when the Dual Strike controller is
activated (if the machine it is plugged in is turned on or the controller gets
plugged into the machine), then special functions are activated:
If the Select button is pressed, then configuration mode is entered (see below).

If the Start button is pressed, then firmware update mode is entered (see
below).

If the X/Cross button is pressed and firmware supports PS3 (default), then the
Dual Strike PS3 working mode is activated. Otherwise if the Y/Circle button is
pressed and firmware supports Wii, then the Dual Strike Wii working mode is
activated. Otherwise if the A/Squard button is pressed, then the pass-through
working mode is activated. Otherwise the default working mode is activated.

If the joystick is moved to the up direction, the joystick is acting as a
digital pad when in Dual Strike working mode (default). If the joystick is moved
to the left direction, the joystick is acting as a left analogue stick when in
Dual Strike working mode. If the joystick is moved to the right direction, the
joystick is acting as a right analogue stick when in Dual Strike working mode.


Configuration Mode
==================
In the configuration mode the behaviour of the Dual Strike can be changed,
see Startup Behaviour for how to enter it. Leave it by pressing Start.

While in configuration mode pressing a button and/or a joystick direction,
changes part of the configuration:

Dual Strike default stick mode:
-------------------------------
Up    = digital pad only (precedence over Left and Right) [default]
Left  = left analogue stick only
Right = right analogue stick only
Down  = activate digital pad additionallly to left or right analogue stick

Default Working Mode:
---------------------
Button: LK
Left  = Dual Strike PS3 [default] if firmware supports PS3
Up    = Dual Strike Wii if firmware supports Wii
Right = pass-through

revert to defaults:
-------------------
Button: MK

Start+Select=Home:
------------------
Button: LP
Left  = disabled [default]
Right = enabled

Extra Pins Mode
---------------
Button: MP
Up    = deactivated (precedence over Left and Right) [default]
Left  = read joystick mode switch (precedence over Down)
        S3 and S4 have to be connected to a triple switch
Right = emulate joystick mode switch for pass-through (precedence over Down)
        S3 and S4 have to be connected to joystick mode pins on the 
		pass-through PCB
Down  = inverted triggers for pass-through
        S3 and S4 have to be connected to trigger pins with active high on the
        pass-through PCB        


Wiimote Adapter
===============
The Dual Strike can act as a Wii Classic Controller when connected to a
Wiimote. You need a passive adapter from USB to the Wiimote extension port
for this working mode, its composition is detailed below.

If you get your Wiimote plug and cable of a working Wii extension (Nunchuck,
Classic Controller, ...), you can add a USB jack to the extension's side of
the cable. So you can continue using the extension by utilising your adapter.

Wiimote extension port layout (as seen looking on the Wiimote bottom): 
+---___---+
| 1  2  3 |
| ======= |
| 4  5  6 |
+---------+
1: Ground [white]
2: No Connection
3: SCL [yellow]
4: SDA [green]
5: Device Detect, in the connector wired to VCC [red]
6: VCC (3.3V) [red]

USB type B port layout (as seen looking at the port):
   --
 /    \
| 2  1 |
| ==== |
| 3  4 |
+------+
1: VCC (5V) [red]
2: D- [white]
3: D+ [green]
3: Ground [black]

Your adapter has to make the following connections:

USB Wires      | Wiimote Wires
---------------+---------------
D-     [white] | SCL    [yellow]
D+     [green] | SDA    [green]
VCC    [red]   | VCC    [red]
Ground [black] | Ground [white]

The adapter should be connected to the USB port before plugging it into
the Wiimote.
Alternatively you can connect the USB wires VCC and Ground to a USB type A plug
(creating a Y adapter) to use the standard USB voltage from another source
(e.g. USB AC adapter).


Firmware
========
To update the firmware of your Dual Strike press and hold Start to switch to the
firmware update mode, then you can execute the firmware update file to update
the firmware (two command prompt windows should open with UAC, one without UAC).
After releasing the Start button, firmware update mode is left.
After updating the firmware it is strongly recommended to enter configuration
mode and load the defaults!

The firmware update files are the following:

Filename                    | Dual Strike type | Functionality
----------------------------+------------------+--------------
update_firmware.bat         | classic          | PS3
update_firmware_smd.bat     | SMD              | PS3
update_firmware_wii.bat     | classic          | Wii
update_firmware_smd_wii.bat | SMD              | Wii

