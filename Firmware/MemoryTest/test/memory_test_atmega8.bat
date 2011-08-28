@ECHO OFF
del report.txt

ECHO Memory Test for Dual Strike
ECHO ===========================
ECHO .

ECHO Put your Dual Strike into firmware update mode by plugging it in and holding 
ECHO Start until firmware update is finished.
PAUSE
MemoryTest\bootloadHID -r MemoryTest\firmware_atmega8_0x00-filled.hex
ECHO Leave firmware update mode by releasing Start.
PAUSE
MemoryTest\memoryTool>>report.txt
MemoryTest\memoryTool -de eeprom_backup.shex
MemoryTest\memoryTool -df flash.shex
ECHO Firmware ATmega8 0x00-filled>>report.txt
ECHO ==============================>>report.txt
memoryTool -c flash.shex firmware_atmega8_0x00-filled.shex>>report.txt

ECHO Put your Dual Strike into firmware update mode by plugging it in and holding 
ECHO Start until firmware update is finished.
PAUSE
MemoryTest\bootloadHID -r MemoryTest\firmware_atmega8_0xFF-filled.hex
ECHO Leave firmware update mode by releasing Start.
PAUSE
MemoryTest\memoryTool -df flash.shex
ECHO.>>report.txt
ECHO Firmware ATmega8 0xFF-filled>>report.txt
ECHO ==============================>>report.txt
MemoryTest\memoryTool -c flash.shex MemoryTest\firmware_atmega8_0xFF-filled.shex>>report.txt

memoryTool -e MemoryTest\eeprom_0x00-filled.hex
memoryTool -de eeprom.shex
ECHO.>>report.txt
ECHO EEPROM 0x00-filled>>report.txt
ECHO ==================>>report.txt
MemoryTest\memoryTool -c eeprom.shex MemoryTest\eeprom_0x00-filled.shex>>report.txt

memoryTool -e MemoryTest\eeprom_0xFF-filled.hex
memoryTool -de eeprom.shex
ECHO.>>report.txt
ECHO EEPROM 0xFF-filled>>report.txt
ECHO ==================>>report.txt
MemoryTest\memoryTool -c eeprom.shex MemoryTest\eeprom_0xFF-filled.shex>>report.txt

MemoryTest\memoryTool -e eeprom_backup.shex
del eeprom_backup.shex
del eeprom.shex
del flash.shex

ECHO Results are in the created file "report.txt" in the current 
ECHO directory. Please reinstall previously installed firmware.
PAUSE