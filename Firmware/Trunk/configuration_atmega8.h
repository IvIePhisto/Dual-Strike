/*
 * GENERATED CONFIGURATION HEADER FILE
 * Title:   Dual Strike Configuration (ATmega8)
 * Device:  1
 * Version: 3
 */
 
#ifndef __configuration_h_included__
#define __configuration_h_included__

#include <avr/eeprom.h> /* EEPROM functions */
#include <stdint.h> /* C99 typedefs */ 

#define CONFIG_DEVICE (uint8_t)1
#define CONFIG_VERSION (uint8_t)3
#define CONFIG_BYTE_WIDTH 2
#define CONFIG_EMPTY 0xFF
#define CONFIG_DEF_1 0b0011100
#define CONFIG_DEF_2 0b000000

#define CONFIG_SET_DEFAULTS(CONFIG)\
  CONFIG[0] = CONFIG_DEVICE;\
  CONFIG[1] = CONFIG_VERSION;\
  CONFIG[2] = CONFIG_DEF_1;\
  CONFIG[3] = CONFIG_DEF_2;\

/* 
 * IMPORTANT:
 * You must use the following macro in your main module to declare the variables
 * "config" and "config_EEPROM". 
 */  
#define CFG_DECLARATION \
uint8_t config_EEPROM[CONFIG_BYTE_WIDTH + 2] EEMEM = {CONFIG_DEVICE, CONFIG_VERSION, CONFIG_DEF_1, CONFIG_DEF_2}; \
uint8_t config[CONFIG_BYTE_WIDTH + 2] = {CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY};

/*
 * Use the following macro to access the configuration from non-main-modules:
 */
#define CFG_EXTERN_DECLARATION extern uint8_t config[CONFIG_BYTE_WIDTH + 2];
  
/* CONFIG TESTS: */
#define CFG_DEF_WORK_MODE_AUTODETECT (((config[0 + 2] >> 0) & 0b00000011)  == 0b00)
#define CFG_DEF_WORK_MODE_PS3 (((config[0 + 2] >> 0) & 0b00000011)  == 0b01)
#define CFG_DEF_WORK_MODE_PC (((config[0 + 2] >> 0) & 0b00000011)  == 0b10)
#define CFG_DEF_WORK_MODE_PT (((config[0 + 2] >> 0) & 0b00000011)  == 0b11)
#define CFG_WORK_MODE_PS3_ENABLED (config[0 + 2] & (1<<2))
#define CFG_WORK_MODE_PC_ENABLED (config[0 + 2] & (1<<3))
#define CFG_WORK_MODE_PT_ENABLED (config[0 + 2] & (1<<4))
#define CFG_DIGITAL_PAD (((config[0 + 2] >> 5) & 0b00000011)  == 0b00)
#define CFG_LEFT_STICK (((config[0 + 2] >> 5) & 0b00000011)  == 0b01)
#define CFG_RIGHT_STICK (((config[0 + 2] >> 5) & 0b00000011)  == 0b10)
#define CFG_META_BUTTON_START (((config[1 + 2] >> 0) & 0b00000001)  == 0b0)
#define CFG_META_BUTTON_SELECT (((config[1 + 2] >> 0) & 0b00000001)  == 0b1)
#define CFG_HOME_EMU (config[1 + 2] & (1<<1))
#define CFG_EMU_4X (config[1 + 2] & (1<<2))
#define CFG_NO_EXTRA_PINS (((config[1 + 2] >> 3) & 0b00000111)  == 0b000)
#define CFG_JOYSTICK_SWITCH_READ (((config[1 + 2] >> 3) & 0b00000111)  == 0b001)
#define CFG_JOYSTICK_SWITCH_EMU (((config[1 + 2] >> 3) & 0b00000111)  == 0b010)
#define CFG_INVERTED_TRIGGERS (((config[1 + 2] >> 3) & 0b00000111)  == 0b011)
#define CFG_X3_READ (((config[1 + 2] >> 3) & 0b00000111)  == 0b100)

/* CONFIG SETTERS: */
#define CFG_SET_DEF_WORK_MODE_AUTODETECT(CONFIG) CONFIG[0 + 2] &= ~(0b11 << 0);
#define CFG_SET_DEF_WORK_MODE_PS3(CONFIG) CONFIG[0 + 2] |= (0b01 << 0); CONFIG[0 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_DEF_WORK_MODE_PC(CONFIG) CONFIG[0 + 2] |= (0b10 << 0); CONFIG[0 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_DEF_WORK_MODE_PT(CONFIG) CONFIG[0 + 2] |= (0b11 << 0);
#define CFG_ENABLE_WORK_MODE_PS3_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<2);
#define CFG_DISABLE_WORK_MODE_PS3_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<2);
#define CFG_ENABLE_WORK_MODE_PC_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<3);
#define CFG_DISABLE_WORK_MODE_PC_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<3);
#define CFG_ENABLE_WORK_MODE_PT_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<4);
#define CFG_DISABLE_WORK_MODE_PT_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<4);
#define CFG_SET_DIGITAL_PAD(CONFIG) CONFIG[0 + 2] &= ~(0b11 << 5);
#define CFG_SET_LEFT_STICK(CONFIG) CONFIG[0 + 2] |= (0b01 << 5); CONFIG[0 + 2] &= ((0b01 << 5)| ~(0b10 << 5));
#define CFG_SET_RIGHT_STICK(CONFIG) CONFIG[0 + 2] |= (0b10 << 5); CONFIG[0 + 2] &= ((0b10 << 5)| ~(0b01 << 5));
#define CFG_SET_META_BUTTON_START(CONFIG) CONFIG[1 + 2] &= ~(0b1 << 0);
#define CFG_SET_META_BUTTON_SELECT(CONFIG) CONFIG[1 + 2] |= (0b1 << 0);
#define CFG_ENABLE_HOME_EMU(CONFIG) CONFIG[1 + 2] |= (1<<1);
#define CFG_DISABLE_HOME_EMU(CONFIG) CONFIG[1 + 2] &= ~(1<<1);
#define CFG_ENABLE_EMU_4X(CONFIG) CONFIG[1 + 2] |= (1<<2);
#define CFG_DISABLE_EMU_4X(CONFIG) CONFIG[1 + 2] &= ~(1<<2);
#define CFG_SET_NO_EXTRA_PINS(CONFIG) CONFIG[1 + 2] &= ~(0b111 << 3);
#define CFG_SET_JOYSTICK_SWITCH_READ(CONFIG) CONFIG[1 + 2] |= (0b001 << 3); CONFIG[1 + 2] &= ((0b001 << 3)| ~(0b110 << 3));
#define CFG_SET_JOYSTICK_SWITCH_EMU(CONFIG) CONFIG[1 + 2] |= (0b010 << 3); CONFIG[1 + 2] &= ((0b010 << 3)| ~(0b101 << 3));
#define CFG_SET_INVERTED_TRIGGERS(CONFIG) CONFIG[1 + 2] |= (0b011 << 3); CONFIG[1 + 2] &= ((0b011 << 3)| ~(0b100 << 3));
#define CFG_SET_X3_READ(CONFIG) CONFIG[1 + 2] |= (0b100 << 3); CONFIG[1 + 2] &= ((0b100 << 3)| ~(0b011 << 3));

#endif
