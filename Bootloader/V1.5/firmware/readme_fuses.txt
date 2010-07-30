NOTE: If you are just updating the firmware, you don't need to worry about any of this.

Fuse settings for the ATmega168
===============================

Debug
-----
extended:	0xf8
high:		0xdf
low:		0xef

This is equivalent to the following settings in AVR Studio ("X" means checked, "O"
means unchecked, "?" means check with question mark with gray background):

BOOTSZ		"Boot Flash size=1024 words start address=$1C00"
BOOTRST		X
RSTDISBL	O
DWEN		O
SPIEN		?
WDTON		O
EESAVE		O
BODLEVEL	"Brown-out detection disabled"
CKDIV		8
CKOUT		O
SUT_CKSEL	"Ext. Crystal Osc. 8.0-   MHz; Start-up time PWRDWN/RESET: 16 CK/14 CK + 4.1 ms"

Disable Reset Line
------------------
extended:	0xf8
high:		0x5f
low:		0xef

In AVR Studio it is the same as above, except:
RSTDIBL		X


To program with avrdude use the following command line:

> avrdude -c <your programmer> -p atmega168 -U lfuse:w:0xef:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m

Ignore warnings about the efuse not being set properly, it is a limitation of avrdude.