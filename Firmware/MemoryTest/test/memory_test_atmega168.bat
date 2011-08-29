@ECHO OFF
del report.txt

ECHO Memory Test for Dual Strike
ECHO ===========================
ECHO .

ECHO Please backup your Dual Strike's configuration with the Configuration Editor
ECHO before running this test, as it is overwritten.
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO Put your Dual Strike into firmware update mode by plugging it in while
ECHO holding Start.
PAUSE
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MemoryTest\bootloadHID -r MemoryTest\firmware_atmega168_0x00-filled.hex
IF ERRORLEVEL 1 GOTO ErrorFW
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PAUSE

MemoryTest\memoryTool>>report.txt
IF ERRORLEVEL 1 GOTO ErrorMT
MemoryTest\memoryTool -df flash.shex
ECHO Firmware ATmega168 0x00-filled>>report.txt
ECHO ==============================>>report.txt
MemoryTest\memoryTool -c flash.shex firmware_atmega168_0x00-filled.shex>>report.txt

ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO Put your Dual Strike into firmware update mode by plugging it in while
ECHO holding Start.
PAUSE
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MemoryTest\bootloadHID -r MemoryTest\firmware_atmega168_0xFF-filled.hex
IF ERRORLEVEL 1 GOTO ErrorFW
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PAUSE

MemoryTest\memoryTool -df flash.shex
IF ERRORLEVEL 1 GOTO ErrorMT
ECHO.>>report.txt
ECHO Firmware ATmega168 0xFF-filled>>report.txt
ECHO ==============================>>report.txt
MemoryTest\memoryTool -c flash.shex MemoryTest\firmware_atmega168_0xFF-filled.shex>>report.txt

MemoryTest\memoryTool -e MemoryTest\eeprom_0x00-filled.hex
MemoryTest\memoryTool -de eeprom.shex
ECHO.>>report.txt
ECHO EEPROM 0x00-filled>>report.txt
ECHO ==================>>report.txt
MemoryTest\memoryTool -c eeprom.shex MemoryTest\eeprom_0x00-filled.shex>>report.txt

MemoryTest\memoryTool -e MemoryTest\eeprom_0xFF-filled.hex
MemoryTest\memoryTool -de eeprom.shex
ECHO.>>report.txt
ECHO EEPROM 0xFF-filled>>report.txt
ECHO ==================>>report.txt
MemoryTest\memoryTool -c eeprom.shex MemoryTest\eeprom_0xFF-filled.shex>>report.txt

del eeprom.shex
del flash.shex

MemoryTest\FindLineInFile report.txt "Mismatches (<address>: <value A> <value B>):"
IF ERRORLEVEL 1 GOTO Fine
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO The memory is probably corrupted, please send the created file "report.txt" to
ECHO the Dual Strike support.
GOTO End

:ErrorFW
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO ERROR: Firmware could not be installed.
GOTO End

:ErrorMT
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO ERROR: Could not communicate with Memory Tool firmware.
GOTO End

:Fine
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO No errors found. Reinstall Dual Strike firmware and restore the configuration.

:End
PAUSE