@ECHO OFF
del firmware_update\*.hex
copy build\dual_strike_firmware.hex firmware_update\dual_strike_firmware_V%1.hex
copy build\dual_strike_smd_firmware.hex firmware_update\dual_strike_smd_firmware_V%1.hex
copy build\dual_strike_firmware_wii.hex firmware_update\dual_strike_firmware_V%1_wii.hex
copy build\dual_strike_smd_firmware_wii.hex firmware_update\dual_strike_smd_firmware_V%1_wii.hex

ECHO @ECHO OFF>firmware_update\update_firmware.bat
ECHO updateds.exe -r "dual_strike_firmware_V%1.hex">>firmware_update\update_firmware.bat
ECHO Pause>>firmware_update\update_firmware.bat

ECHO @ECHO OFF>firmware_update\update_firmware_smd.bat
ECHO updateds.exe -r "dual_strike_smd_firmware_V%1.hex">>firmware_update\update_firmware_smd.bat
ECHO Pause>>firmware_update\update_firmware_smd.bat

ECHO @ECHO OFF>firmware_update\update_firmware_wii.bat
ECHO updateds.exe -r "dual_strike_firmware_V%1_wii.hex">>firmware_update\update_firmware_wii.bat
ECHO Pause>>firmware_update\update_firmware_wii.bat

ECHO @ECHO OFF>firmware_update\update_firmware_smd_wii.bat
ECHO updateds.exe -r "dual_strike_smd_firmware_V%1_wii.hex">>firmware_update\update_firmware_smd_wii.bat
ECHO Pause>>firmware_update\update_firmware_smd_wii.bat