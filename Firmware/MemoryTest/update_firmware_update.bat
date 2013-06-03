@ECHO OFF
del firmware_update\*.hex
del firmware_update\*.bat

copy build\dual_strike_firmware_atmega168.hex firmware_update\memory_test_firmware_atmega168.hex
copy build\dual_strike_firmware_atmega8.hex firmware_update\memory_test_firmware_atmega8.hex

ECHO @ECHO OFF>firmware_update\update_firmware_atmega168.bat
ECHO bootloadHID.exe -r "memory_test_firmware_atmega168.hex">>firmware_update\update_firmware_atmega168.bat
ECHO Pause>>firmware_update\update_firmware_atmega168.bat

ECHO @ECHO OFF>firmware_update\update_firmware_atmega8.bat
ECHO bootloadHID.exe -r "memory_test_firmware_atmega8_V%1.hex">>firmware_update\update_firmware_atmega8.bat
ECHO Pause>>firmware_update\update_firmware_atmega8.bat
