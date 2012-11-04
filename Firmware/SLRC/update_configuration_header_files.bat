@echo off
cd configuration_editor\mcc
java -jar mcc-1.2.0.jar -ghf configuration_atmega8.xml ..\..\configuration_atmega8.h
java -jar mcc-1.2.0.jar -ghf configuration_atmega168.xml ..\..\configuration_atmega168.h
pause