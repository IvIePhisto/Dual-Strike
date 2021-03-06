/*
 * GENERATED CONFIGURATION HEADER FILE
 * Title:   Dual Strike Configuration
 * Device:  1
 * Version: 1
 */
 
#ifndef __configuration_h_included__
#define __configuration_h_included__

#include <avr/eeprom.h> /* EEPROM functions */
#include <stdint.h> /* C99 typedefs */ 

#define CONFIG_DEVICE (uint8_t)1
#define CONFIG_VERSION (uint8_t)1
#define CONFIG_BYTE_WIDTH 1
#define CONFIG_EMPTY 0xFF
#define CONFIG_DEF_1 0b000000

#define CONFIG_SET_DEFAULTS(CONFIG)\
  CONFIG[0] = CONFIG_DEVICE;\
  CONFIG[1] = CONFIG_VERSION;\
  CONFIG[2] = CONFIG_DEF_1;\

/* 
 * IMPORTANT:
 * You must use the following macro in your main module to declare the variables
 * "config" and "config_EEPROM". 
 */  
#define CFG_DECLARATION \
uint8_t config_EEPROM[CONFIG_BYTE_WIDTH + 2] EEMEM = {CONFIG_DEVICE, CONFIG_VERSION, CONFIG_DEF_1}; \
uint8_t config[CONFIG_BYTE_WIDTH + 2] = {CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY};

/*
 * Use the following macro to access the configuration from non-main-modules:
 */
#define CFG_EXTERN_DECLARATION extern uint8_t config[CONFIG_BYTE_WIDTH + 2];
  
/* CONFIG TESTS: */
#define CFG_DEF_WORK_MODE_PS3 (((config[0 + 2] >> 0) & 0b00000001)  == 0b0)
#define CFG_DEF_WORK_MODE_PT (((config[0 + 2] >> 0) & 0b00000001)  == 0b1)
#define CFG_DIGITAL_PAD (((config[0 + 2] >> 1) & 0b00000011)  == 0b00)
#define CFG_LEFT_STICK (((config[0 + 2] >> 1) & 0b00000011)  == 0b01)
#define CFG_RIGHT_STICK (((config[0 + 2] >> 1) & 0b00000011)  == 0b10)
#define CFG_HOME_EMU (config[0 + 2] & (1<<3))
#define CFG_NO_EXTRA_PINS (((config[0 + 2] >> 4) & 0b00000011)  == 0b00)
#define CFG_JOYSTICK_SWITCH_READ (((config[0 + 2] >> 4) & 0b00000011)  == 0b01)
#define CFG_JOYSTICK_SWITCH_EMU (((config[0 + 2] >> 4) & 0b00000011)  == 0b10)
#define CFG_INVERTED_TRIGGERS (((config[0 + 2] >> 4) & 0b00000011)  == 0b11)

/* CONFIG SETTERS: */
#define CFG_SET_DEF_WORK_MODE_PS3(CONFIG) CONFIG[0 + 2] &= ~(0b1 << 0);
#define CFG_SET_DEF_WORK_MODE_PT(CONFIG) CONFIG[0 + 2] |= (0b1 << 0);
#define CFG_SET_DIGITAL_PAD(CONFIG) CONFIG[0 + 2] &= ~(0b11 << 1);
#define CFG_SET_LEFT_STICK(CONFIG) CONFIG[0 + 2] |= (0b01 << 1); CONFIG[0 + 2] &= ((0b01 << 1)| ~(0b10 << 1));
#define CFG_SET_RIGHT_STICK(CONFIG) CONFIG[0 + 2] |= (0b10 << 1); CONFIG[0 + 2] &= ((0b10 << 1)| ~(0b01 << 1));
#define CFG_ENABLE_HOME_EMU(CONFIG) CONFIG[0 + 2] |= (1<<3);
#define CFG_DISABLE_HOME_EMU(CONFIG) CONFIG[0 + 2] &= ~(1<<3);
#define CFG_SET_NO_EXTRA_PINS(CONFIG) CONFIG[0 + 2] &= ~(0b11 << 4);
#define CFG_SET_JOYSTICK_SWITCH_READ(CONFIG) CONFIG[0 + 2] |= (0b01 << 4); CONFIG[0 + 2] &= ((0b01 << 4)| ~(0b10 << 4));
#define CFG_SET_JOYSTICK_SWITCH_EMU(CONFIG) CONFIG[0 + 2] |= (0b10 << 4); CONFIG[0 + 2] &= ((0b10 << 4)| ~(0b01 << 4));
#define CFG_SET_INVERTED_TRIGGERS(CONFIG) CONFIG[0 + 2] |= (0b11 << 4);

#endif
