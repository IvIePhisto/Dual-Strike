@ECHO OFF
del report.txt

ECHO Memory Test for Dual Strike
ECHO ===========================
ECHO .
ECHO Put your Dual Strike into firmware update mode by plugging it in while
ECHO holding Start.
PAUSE
bootloadHID -r firmware_atmega168_0x00-filled.hex

ECHO Firmware ATmega168 0x00-filled>>report.txt
ECHO ==============================>>report.txt
memoryTool -df flash.shex>>report.txt
memoryTool -c flash.shex firmware_atmega168_0x00-filled.shex>>report.txt

ECHO Put your Dual Strike into firmware update mode one more time.
PAUSE
bootloadHID -r firmware_atmega168_0xFF-filled.hex

ECHO.>>report.txt
ECHO Firmware ATmega168 0xFF-filled>>report.txt
ECHO ==============================>>report.txt
memoryTool -df flash.shex>>report.txt
memoryTool -c flash.shex firmware_atmega168_0xFF-filled.shex>>report.txt

ECHO.>>report.txt
ECHO EEPROM 0x00-filled>>report.txt
ECHO ==================>>report.txt
memoryTool -e eeprom_0x00-filled.hex>>report.txt
memoryTool -de eeprom.shex>>report.txt
memoryTool -c eeprom.shex eeprom_0x00-filled.shex>>report.txt

ECHO.>>report.txt
ECHO EEPROM 0xFF-filled>>report.txt
ECHO ==================>>report.txt
memoryTool -e eeprom_0xFF-filled.hex>>report.txt
memoryTool -de eeprom.shex>>report.txt
memoryTool -c eeprom.shex eeprom_0xFF-filled.shex>>report.txt

del eeprom.shex
del flash.shex

ECHO Results are in the created file "report.txt" in the current 
ECHO directory.
PAUSE