@ECHO OFF
del report.txt

ECHO Memory Test for Dual Strike
ECHO ===========================
ECHO .
ECHO Put your Dual Strike into firmware update mode by plugging it in and holding 
ECHO Start until firmware update is finished.
PAUSE
bootloadHID -r firmware_atmega8_0x00-filled.hex
ECHO Leave firmware update mode by releasing Start.
PAUSE

ECHO Firmware ATmega8 0x00-filled>>report.txt
ECHO ============================>>report.txt
memoryTool -df flash.shex>>report.txt
memoryTool -c flash.shex firmware_atmega8_0x00-filled.shex>>report.txt

ECHO Put your Dual Strike into firmware update mode one more time.
PAUSE
bootloadHID -r firmware_atmega8_0xFF-filled.hex
ECHO Leave firmware update mode by releasing Start.
PAUSE

ECHO.>>report.txt
ECHO Firmware ATmega8 0xFF-filled>>report.txt
ECHO ============================>>report.txt
memoryTool -df flash.shex>>report.txt
memoryTool -c flash.shex firmware_atmega8_0xFF-filled.shex>>report.txt

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