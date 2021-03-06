@ECHO OFF
del firmware_update\*.hex
del firmware_update\*.bat

copy build\dual_strike_firmware_atmega168.hex firmware_update\dual_strike_firmware_atmega168_V%1.hex
REM copy build\dual_strike_firmware_atmega8.hex firmware_update\dual_strike_firmware_atmega8_V%1.hex
copy build\dual_strike_firmware_atmega168_smd.hex firmware_update\dual_strike_firmware_atmega168_smd_V%1.hex
REM copy build\dual_strike_firmware_atmega8_smd.hex firmware_update\dual_strike_firmware_atmega8_smd_V%1.hex

ECHO @ECHO OFF>firmware_update\update_firmware_atmega168.bat
ECHO bootloadHID.exe -r "dual_strike_firmware_atmega168_V%1.hex">>firmware_update\update_firmware_atmega168.bat
ECHO Pause>>firmware_update\update_firmware_atmega168.bat

REM ECHO @ECHO OFF>firmware_update\update_firmware_atmega8.bat
REM ECHO bootloadHID.exe -r "dual_strike_firmware_atmega8_V%1.hex">>firmware_update\update_firmware_atmega8.bat
REM ECHO Pause>>firmware_update\update_firmware_atmega8.bat

ECHO @ECHO OFF>firmware_update\update_firmware_atmega168_smd.bat
ECHO bootloadHID.exe -r "dual_strike_firmware_atmega168_smd_V%1.hex">>firmware_update\update_firmware_atmega168_smd.bat
ECHO Pause>>firmware_update\update_firmware_atmega168_smd.bat

REM ECHO @ECHO OFF>firmware_update\update_firmware_atmega8_smd.bat
REM ECHO bootloadHID.exe -r "dual_strike_firmware_atmega8_smd_V%1.hex">>firmware_update\update_firmware_atmega8_smd.bat
REM ECHO Pause>>firmware_update\update_firmware_atmega8_smd.bat
