/*
 * GENERATED CONFIGURATION HEADER FILE
 * Title:   Dual Strike Configuration
 * Device:  2
 * Version: 1
 */
 
#ifndef __configuration_h_included__
#define __configuration_h_included__

#include <avr/eeprom.h> /* EEPROM functions */
#include <stdint.h> /* C99 typedefs */ 

#define CONFIG_DEVICE (uint8_t)2
#define CONFIG_VERSION (uint8_t)1
#define CONFIG_BYTE_WIDTH 16
#define CONFIG_EMPTY 0xFF
#define CONFIG_DEF_1 0b0111100
#define CONFIG_DEF_2 0b1100000
#define CONFIG_DEF_3 0b1101000
#define CONFIG_DEF_4 0b0001000
#define CONFIG_DEF_5 0b0010101
#define CONFIG_DEF_6 0b1011000
#define CONFIG_DEF_7 0b1000110
#define CONFIG_DEF_8 0b1101000
#define CONFIG_DEF_9 0b1010000
#define CONFIG_DEF_10 0b0000110
#define CONFIG_DEF_11 0b1000100
#define CONFIG_DEF_12 0b1000110
#define CONFIG_DEF_13 0b0101100
#define CONFIG_DEF_14 0b0100011
#define CONFIG_DEF_15 0b0110100
#define CONFIG_DEF_16 0b100

#define CONFIG_SET_DEFAULTS(CONFIG)\
  CONFIG[0] = CONFIG_DEVICE;\
  CONFIG[1] = CONFIG_VERSION;\
  CONFIG[2] = CONFIG_DEF_1;\
  CONFIG[3] = CONFIG_DEF_2;\
  CONFIG[4] = CONFIG_DEF_3;\
  CONFIG[5] = CONFIG_DEF_4;\
  CONFIG[6] = CONFIG_DEF_5;\
  CONFIG[7] = CONFIG_DEF_6;\
  CONFIG[8] = CONFIG_DEF_7;\
  CONFIG[9] = CONFIG_DEF_8;\
  CONFIG[10] = CONFIG_DEF_9;\
  CONFIG[11] = CONFIG_DEF_10;\
  CONFIG[12] = CONFIG_DEF_11;\
  CONFIG[13] = CONFIG_DEF_12;\
  CONFIG[14] = CONFIG_DEF_13;\
  CONFIG[15] = CONFIG_DEF_14;\
  CONFIG[16] = CONFIG_DEF_15;\
  CONFIG[17] = CONFIG_DEF_16;\

/* 
 * IMPORTANT:
 * You must use the following macro in your main module to declare the variables
 * "config" and "config_EEPROM". 
 */  
#define CFG_DECLARATION \
uint8_t config_EEPROM[CONFIG_BYTE_WIDTH + 2] EEMEM = {CONFIG_DEVICE, CONFIG_VERSION, CONFIG_DEF_1, CONFIG_DEF_2, CONFIG_DEF_3, CONFIG_DEF_4, CONFIG_DEF_5, CONFIG_DEF_6, CONFIG_DEF_7, CONFIG_DEF_8, CONFIG_DEF_9, CONFIG_DEF_10, CONFIG_DEF_11, CONFIG_DEF_12, CONFIG_DEF_13, CONFIG_DEF_14, CONFIG_DEF_15, CONFIG_DEF_16}; \
uint8_t config[CONFIG_BYTE_WIDTH + 2] = {CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY, CONFIG_EMPTY};

/*
 * Use the following macro to access the configuration from non-main-modules:
 */
#define CFG_EXTERN_DECLARATION extern uint8_t config[CONFIG_BYTE_WIDTH + 2];
  
/* CONFIG TESTS: */
#define CFG_DEF_WORK_MODE_PS3 (((config[0 + 2] >> 0) & 0b00000011)  == 0b00)
#define CFG_DEF_WORK_MODE_MAME (((config[0 + 2] >> 0) & 0b00000011)  == 0b01)
#define CFG_DEF_WORK_MODE_XBOX (((config[0 + 2] >> 0) & 0b00000011)  == 0b10)
#define CFG_DEF_WORK_MODE_PT (((config[0 + 2] >> 0) & 0b00000011)  == 0b11)
#define CFG_WORK_MODE_PS3_ENABLED (config[0 + 2] & (1<<2))
#define CFG_WORK_MODE_MAME_ENABLED (config[0 + 2] & (1<<3))
#define CFG_WORK_MODE_XBOX_ENABLED (config[0 + 2] & (1<<4))
#define CFG_WORK_MODE_PT_ENABLED (config[0 + 2] & (1<<5))
#define CFG_DIGITAL_PAD ((((config[0 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[1 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_LEFT_STICK ((((config[0 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[1 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_RIGHT_STICK ((((config[0 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[1 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_DEFAULT_PLAYER_1 (((config[1 + 2] >> 1) & 0b00000001)  == 0b0)
#define CFG_MAME_DEFAULT_PLAYER_2 (((config[1 + 2] >> 1) & 0b00000001)  == 0b1)
#define CFG_HOME_EMU (config[1 + 2] & (1<<2))
#define CFG_NO_EXTRA_PINS (((config[1 + 2] >> 3) & 0b00000011)  == 0b00)
#define CFG_JOYSTICK_SWITCH_READ (((config[1 + 2] >> 3) & 0b00000011)  == 0b01)
#define CFG_JOYSTICK_SWITCH_EMU (((config[1 + 2] >> 3) & 0b00000011)  == 0b10)
#define CFG_INVERTED_TRIGGERS (((config[1 + 2] >> 3) & 0b00000011)  == 0b11)
#define CFG_MAME_BL1_LK_NOTHING ((((config[1 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_LK_BUTTON1 ((((config[1 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_LK_BUTTON2 ((((config[1 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_LK_BUTTON3 ((((config[1 + 2] >> 5) & 0b00000011)  == 0b11) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_LK_BUTTON4 ((((config[1 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL1_LK_BUTTON5 ((((config[1 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL1_LK_BUTTON6 ((((config[1 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[2 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL1_MK_NOTHING (((config[2 + 2] >> 1) & 0b00000111)  == 0b000)
#define CFG_MAME_BL1_MK_BUTTON1 (((config[2 + 2] >> 1) & 0b00000111)  == 0b001)
#define CFG_MAME_BL1_MK_BUTTON2 (((config[2 + 2] >> 1) & 0b00000111)  == 0b010)
#define CFG_MAME_BL1_MK_BUTTON3 (((config[2 + 2] >> 1) & 0b00000111)  == 0b011)
#define CFG_MAME_BL1_MK_BUTTON4 (((config[2 + 2] >> 1) & 0b00000111)  == 0b100)
#define CFG_MAME_BL1_MK_BUTTON5 (((config[2 + 2] >> 1) & 0b00000111)  == 0b101)
#define CFG_MAME_BL1_MK_BUTTON6 (((config[2 + 2] >> 1) & 0b00000111)  == 0b110)
#define CFG_MAME_BL1_HK_NOTHING (((config[2 + 2] >> 4) & 0b00000111)  == 0b000)
#define CFG_MAME_BL1_HK_BUTTON1 (((config[2 + 2] >> 4) & 0b00000111)  == 0b001)
#define CFG_MAME_BL1_HK_BUTTON2 (((config[2 + 2] >> 4) & 0b00000111)  == 0b010)
#define CFG_MAME_BL1_HK_BUTTON3 (((config[2 + 2] >> 4) & 0b00000111)  == 0b011)
#define CFG_MAME_BL1_HK_BUTTON4 (((config[2 + 2] >> 4) & 0b00000111)  == 0b100)
#define CFG_MAME_BL1_HK_BUTTON5 (((config[2 + 2] >> 4) & 0b00000111)  == 0b101)
#define CFG_MAME_BL1_HK_BUTTON6 (((config[2 + 2] >> 4) & 0b00000111)  == 0b110)
#define CFG_MAME_BL1_4K_NOTHING (((config[3 + 2] >> 0) & 0b00000111)  == 0b000)
#define CFG_MAME_BL1_4K_BUTTON1 (((config[3 + 2] >> 0) & 0b00000111)  == 0b001)
#define CFG_MAME_BL1_4K_BUTTON2 (((config[3 + 2] >> 0) & 0b00000111)  == 0b010)
#define CFG_MAME_BL1_4K_BUTTON3 (((config[3 + 2] >> 0) & 0b00000111)  == 0b011)
#define CFG_MAME_BL1_4K_BUTTON4 (((config[3 + 2] >> 0) & 0b00000111)  == 0b100)
#define CFG_MAME_BL1_4K_BUTTON5 (((config[3 + 2] >> 0) & 0b00000111)  == 0b101)
#define CFG_MAME_BL1_4K_BUTTON6 (((config[3 + 2] >> 0) & 0b00000111)  == 0b110)
#define CFG_MAME_BL1_LP_NOTHING (((config[3 + 2] >> 3) & 0b00000111)  == 0b000)
#define CFG_MAME_BL1_LP_BUTTON1 (((config[3 + 2] >> 3) & 0b00000111)  == 0b001)
#define CFG_MAME_BL1_LP_BUTTON2 (((config[3 + 2] >> 3) & 0b00000111)  == 0b010)
#define CFG_MAME_BL1_LP_BUTTON3 (((config[3 + 2] >> 3) & 0b00000111)  == 0b011)
#define CFG_MAME_BL1_LP_BUTTON4 (((config[3 + 2] >> 3) & 0b00000111)  == 0b100)
#define CFG_MAME_BL1_LP_BUTTON5 (((config[3 + 2] >> 3) & 0b00000111)  == 0b101)
#define CFG_MAME_BL1_LP_BUTTON6 (((config[3 + 2] >> 3) & 0b00000111)  == 0b110)
#define CFG_MAME_BL1_MP_NOTHING ((((config[3 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL1_MP_BUTTON1 ((((config[3 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL1_MP_BUTTON2 ((((config[3 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL1_MP_BUTTON3 ((((config[3 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL1_MP_BUTTON4 ((((config[3 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL1_MP_BUTTON5 ((((config[3 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL1_MP_BUTTON6 ((((config[3 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[4 + 2] >> 0) & 0b00000011)  == 0b11))
#define CFG_MAME_BL1_HP_NOTHING (((config[4 + 2] >> 2) & 0b00000111)  == 0b000)
#define CFG_MAME_BL1_HP_BUTTON1 (((config[4 + 2] >> 2) & 0b00000111)  == 0b001)
#define CFG_MAME_BL1_HP_BUTTON2 (((config[4 + 2] >> 2) & 0b00000111)  == 0b010)
#define CFG_MAME_BL1_HP_BUTTON3 (((config[4 + 2] >> 2) & 0b00000111)  == 0b011)
#define CFG_MAME_BL1_HP_BUTTON4 (((config[4 + 2] >> 2) & 0b00000111)  == 0b100)
#define CFG_MAME_BL1_HP_BUTTON5 (((config[4 + 2] >> 2) & 0b00000111)  == 0b101)
#define CFG_MAME_BL1_HP_BUTTON6 (((config[4 + 2] >> 2) & 0b00000111)  == 0b110)
#define CFG_MAME_BL1_4P_NOTHING ((((config[4 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_4P_BUTTON1 ((((config[4 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_4P_BUTTON2 ((((config[4 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_4P_BUTTON3 ((((config[4 + 2] >> 5) & 0b00000011)  == 0b11) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL1_4P_BUTTON4 ((((config[4 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL1_4P_BUTTON5 ((((config[4 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL1_4P_BUTTON6 ((((config[4 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[5 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL2_LK_NOTHING (((config[5 + 2] >> 1) & 0b00000111)  == 0b000)
#define CFG_MAME_BL2_LK_BUTTON1 (((config[5 + 2] >> 1) & 0b00000111)  == 0b001)
#define CFG_MAME_BL2_LK_BUTTON2 (((config[5 + 2] >> 1) & 0b00000111)  == 0b010)
#define CFG_MAME_BL2_LK_BUTTON3 (((config[5 + 2] >> 1) & 0b00000111)  == 0b011)
#define CFG_MAME_BL2_LK_BUTTON4 (((config[5 + 2] >> 1) & 0b00000111)  == 0b100)
#define CFG_MAME_BL2_LK_BUTTON5 (((config[5 + 2] >> 1) & 0b00000111)  == 0b101)
#define CFG_MAME_BL2_LK_BUTTON6 (((config[5 + 2] >> 1) & 0b00000111)  == 0b110)
#define CFG_MAME_BL2_MK_NOTHING (((config[5 + 2] >> 4) & 0b00000111)  == 0b000)
#define CFG_MAME_BL2_MK_BUTTON1 (((config[5 + 2] >> 4) & 0b00000111)  == 0b001)
#define CFG_MAME_BL2_MK_BUTTON2 (((config[5 + 2] >> 4) & 0b00000111)  == 0b010)
#define CFG_MAME_BL2_MK_BUTTON3 (((config[5 + 2] >> 4) & 0b00000111)  == 0b011)
#define CFG_MAME_BL2_MK_BUTTON4 (((config[5 + 2] >> 4) & 0b00000111)  == 0b100)
#define CFG_MAME_BL2_MK_BUTTON5 (((config[5 + 2] >> 4) & 0b00000111)  == 0b101)
#define CFG_MAME_BL2_MK_BUTTON6 (((config[5 + 2] >> 4) & 0b00000111)  == 0b110)
#define CFG_MAME_BL2_HK_NOTHING (((config[6 + 2] >> 0) & 0b00000111)  == 0b000)
#define CFG_MAME_BL2_HK_BUTTON1 (((config[6 + 2] >> 0) & 0b00000111)  == 0b001)
#define CFG_MAME_BL2_HK_BUTTON2 (((config[6 + 2] >> 0) & 0b00000111)  == 0b010)
#define CFG_MAME_BL2_HK_BUTTON3 (((config[6 + 2] >> 0) & 0b00000111)  == 0b011)
#define CFG_MAME_BL2_HK_BUTTON4 (((config[6 + 2] >> 0) & 0b00000111)  == 0b100)
#define CFG_MAME_BL2_HK_BUTTON5 (((config[6 + 2] >> 0) & 0b00000111)  == 0b101)
#define CFG_MAME_BL2_HK_BUTTON6 (((config[6 + 2] >> 0) & 0b00000111)  == 0b110)
#define CFG_MAME_BL2_4K_NOTHING (((config[6 + 2] >> 3) & 0b00000111)  == 0b000)
#define CFG_MAME_BL2_4K_BUTTON1 (((config[6 + 2] >> 3) & 0b00000111)  == 0b001)
#define CFG_MAME_BL2_4K_BUTTON2 (((config[6 + 2] >> 3) & 0b00000111)  == 0b010)
#define CFG_MAME_BL2_4K_BUTTON3 (((config[6 + 2] >> 3) & 0b00000111)  == 0b011)
#define CFG_MAME_BL2_4K_BUTTON4 (((config[6 + 2] >> 3) & 0b00000111)  == 0b100)
#define CFG_MAME_BL2_4K_BUTTON5 (((config[6 + 2] >> 3) & 0b00000111)  == 0b101)
#define CFG_MAME_BL2_4K_BUTTON6 (((config[6 + 2] >> 3) & 0b00000111)  == 0b110)
#define CFG_MAME_BL2_LP_NOTHING ((((config[6 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL2_LP_BUTTON1 ((((config[6 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL2_LP_BUTTON2 ((((config[6 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL2_LP_BUTTON3 ((((config[6 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL2_LP_BUTTON4 ((((config[6 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL2_LP_BUTTON5 ((((config[6 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL2_LP_BUTTON6 ((((config[6 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[7 + 2] >> 0) & 0b00000011)  == 0b11))
#define CFG_MAME_BL2_MP_NOTHING (((config[7 + 2] >> 2) & 0b00000111)  == 0b000)
#define CFG_MAME_BL2_MP_BUTTON1 (((config[7 + 2] >> 2) & 0b00000111)  == 0b001)
#define CFG_MAME_BL2_MP_BUTTON2 (((config[7 + 2] >> 2) & 0b00000111)  == 0b010)
#define CFG_MAME_BL2_MP_BUTTON3 (((config[7 + 2] >> 2) & 0b00000111)  == 0b011)
#define CFG_MAME_BL2_MP_BUTTON4 (((config[7 + 2] >> 2) & 0b00000111)  == 0b100)
#define CFG_MAME_BL2_MP_BUTTON5 (((config[7 + 2] >> 2) & 0b00000111)  == 0b101)
#define CFG_MAME_BL2_MP_BUTTON6 (((config[7 + 2] >> 2) & 0b00000111)  == 0b110)
#define CFG_MAME_BL2_HP_NOTHING ((((config[7 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL2_HP_BUTTON1 ((((config[7 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL2_HP_BUTTON2 ((((config[7 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL2_HP_BUTTON3 ((((config[7 + 2] >> 5) & 0b00000011)  == 0b11) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL2_HP_BUTTON4 ((((config[7 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL2_HP_BUTTON5 ((((config[7 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL2_HP_BUTTON6 ((((config[7 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[8 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL2_4P_NOTHING (((config[8 + 2] >> 1) & 0b00000111)  == 0b000)
#define CFG_MAME_BL2_4P_BUTTON1 (((config[8 + 2] >> 1) & 0b00000111)  == 0b001)
#define CFG_MAME_BL2_4P_BUTTON2 (((config[8 + 2] >> 1) & 0b00000111)  == 0b010)
#define CFG_MAME_BL2_4P_BUTTON3 (((config[8 + 2] >> 1) & 0b00000111)  == 0b011)
#define CFG_MAME_BL2_4P_BUTTON4 (((config[8 + 2] >> 1) & 0b00000111)  == 0b100)
#define CFG_MAME_BL2_4P_BUTTON5 (((config[8 + 2] >> 1) & 0b00000111)  == 0b101)
#define CFG_MAME_BL2_4P_BUTTON6 (((config[8 + 2] >> 1) & 0b00000111)  == 0b110)
#define CFG_MAME_BL3_LK_NOTHING (((config[8 + 2] >> 4) & 0b00000111)  == 0b000)
#define CFG_MAME_BL3_LK_BUTTON1 (((config[8 + 2] >> 4) & 0b00000111)  == 0b001)
#define CFG_MAME_BL3_LK_BUTTON2 (((config[8 + 2] >> 4) & 0b00000111)  == 0b010)
#define CFG_MAME_BL3_LK_BUTTON3 (((config[8 + 2] >> 4) & 0b00000111)  == 0b011)
#define CFG_MAME_BL3_LK_BUTTON4 (((config[8 + 2] >> 4) & 0b00000111)  == 0b100)
#define CFG_MAME_BL3_LK_BUTTON5 (((config[8 + 2] >> 4) & 0b00000111)  == 0b101)
#define CFG_MAME_BL3_LK_BUTTON6 (((config[8 + 2] >> 4) & 0b00000111)  == 0b110)
#define CFG_MAME_BL3_MK_NOTHING (((config[9 + 2] >> 0) & 0b00000111)  == 0b000)
#define CFG_MAME_BL3_MK_BUTTON1 (((config[9 + 2] >> 0) & 0b00000111)  == 0b001)
#define CFG_MAME_BL3_MK_BUTTON2 (((config[9 + 2] >> 0) & 0b00000111)  == 0b010)
#define CFG_MAME_BL3_MK_BUTTON3 (((config[9 + 2] >> 0) & 0b00000111)  == 0b011)
#define CFG_MAME_BL3_MK_BUTTON4 (((config[9 + 2] >> 0) & 0b00000111)  == 0b100)
#define CFG_MAME_BL3_MK_BUTTON5 (((config[9 + 2] >> 0) & 0b00000111)  == 0b101)
#define CFG_MAME_BL3_MK_BUTTON6 (((config[9 + 2] >> 0) & 0b00000111)  == 0b110)
#define CFG_MAME_BL3_HK_NOTHING (((config[9 + 2] >> 3) & 0b00000111)  == 0b000)
#define CFG_MAME_BL3_HK_BUTTON1 (((config[9 + 2] >> 3) & 0b00000111)  == 0b001)
#define CFG_MAME_BL3_HK_BUTTON2 (((config[9 + 2] >> 3) & 0b00000111)  == 0b010)
#define CFG_MAME_BL3_HK_BUTTON3 (((config[9 + 2] >> 3) & 0b00000111)  == 0b011)
#define CFG_MAME_BL3_HK_BUTTON4 (((config[9 + 2] >> 3) & 0b00000111)  == 0b100)
#define CFG_MAME_BL3_HK_BUTTON5 (((config[9 + 2] >> 3) & 0b00000111)  == 0b101)
#define CFG_MAME_BL3_HK_BUTTON6 (((config[9 + 2] >> 3) & 0b00000111)  == 0b110)
#define CFG_MAME_BL3_4K_NOTHING ((((config[9 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL3_4K_BUTTON1 ((((config[9 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL3_4K_BUTTON2 ((((config[9 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL3_4K_BUTTON3 ((((config[9 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL3_4K_BUTTON4 ((((config[9 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL3_4K_BUTTON5 ((((config[9 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL3_4K_BUTTON6 ((((config[9 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[10 + 2] >> 0) & 0b00000011)  == 0b11))
#define CFG_MAME_BL3_LP_NOTHING (((config[10 + 2] >> 2) & 0b00000111)  == 0b000)
#define CFG_MAME_BL3_LP_BUTTON1 (((config[10 + 2] >> 2) & 0b00000111)  == 0b001)
#define CFG_MAME_BL3_LP_BUTTON2 (((config[10 + 2] >> 2) & 0b00000111)  == 0b010)
#define CFG_MAME_BL3_LP_BUTTON3 (((config[10 + 2] >> 2) & 0b00000111)  == 0b011)
#define CFG_MAME_BL3_LP_BUTTON4 (((config[10 + 2] >> 2) & 0b00000111)  == 0b100)
#define CFG_MAME_BL3_LP_BUTTON5 (((config[10 + 2] >> 2) & 0b00000111)  == 0b101)
#define CFG_MAME_BL3_LP_BUTTON6 (((config[10 + 2] >> 2) & 0b00000111)  == 0b110)
#define CFG_MAME_BL3_MP_NOTHING ((((config[10 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL3_MP_BUTTON1 ((((config[10 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL3_MP_BUTTON2 ((((config[10 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL3_MP_BUTTON3 ((((config[10 + 2] >> 5) & 0b00000011)  == 0b11) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL3_MP_BUTTON4 ((((config[10 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL3_MP_BUTTON5 ((((config[10 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL3_MP_BUTTON6 ((((config[10 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[11 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL3_HP_NOTHING (((config[11 + 2] >> 1) & 0b00000111)  == 0b000)
#define CFG_MAME_BL3_HP_BUTTON1 (((config[11 + 2] >> 1) & 0b00000111)  == 0b001)
#define CFG_MAME_BL3_HP_BUTTON2 (((config[11 + 2] >> 1) & 0b00000111)  == 0b010)
#define CFG_MAME_BL3_HP_BUTTON3 (((config[11 + 2] >> 1) & 0b00000111)  == 0b011)
#define CFG_MAME_BL3_HP_BUTTON4 (((config[11 + 2] >> 1) & 0b00000111)  == 0b100)
#define CFG_MAME_BL3_HP_BUTTON5 (((config[11 + 2] >> 1) & 0b00000111)  == 0b101)
#define CFG_MAME_BL3_HP_BUTTON6 (((config[11 + 2] >> 1) & 0b00000111)  == 0b110)
#define CFG_MAME_BL3_4P_NOTHING (((config[11 + 2] >> 4) & 0b00000111)  == 0b000)
#define CFG_MAME_BL3_4P_BUTTON1 (((config[11 + 2] >> 4) & 0b00000111)  == 0b001)
#define CFG_MAME_BL3_4P_BUTTON2 (((config[11 + 2] >> 4) & 0b00000111)  == 0b010)
#define CFG_MAME_BL3_4P_BUTTON3 (((config[11 + 2] >> 4) & 0b00000111)  == 0b011)
#define CFG_MAME_BL3_4P_BUTTON4 (((config[11 + 2] >> 4) & 0b00000111)  == 0b100)
#define CFG_MAME_BL3_4P_BUTTON5 (((config[11 + 2] >> 4) & 0b00000111)  == 0b101)
#define CFG_MAME_BL3_4P_BUTTON6 (((config[11 + 2] >> 4) & 0b00000111)  == 0b110)
#define CFG_MAME_BL4_LK_NOTHING (((config[12 + 2] >> 0) & 0b00000111)  == 0b000)
#define CFG_MAME_BL4_LK_BUTTON1 (((config[12 + 2] >> 0) & 0b00000111)  == 0b001)
#define CFG_MAME_BL4_LK_BUTTON2 (((config[12 + 2] >> 0) & 0b00000111)  == 0b010)
#define CFG_MAME_BL4_LK_BUTTON3 (((config[12 + 2] >> 0) & 0b00000111)  == 0b011)
#define CFG_MAME_BL4_LK_BUTTON4 (((config[12 + 2] >> 0) & 0b00000111)  == 0b100)
#define CFG_MAME_BL4_LK_BUTTON5 (((config[12 + 2] >> 0) & 0b00000111)  == 0b101)
#define CFG_MAME_BL4_LK_BUTTON6 (((config[12 + 2] >> 0) & 0b00000111)  == 0b110)
#define CFG_MAME_BL4_MK_NOTHING (((config[12 + 2] >> 3) & 0b00000111)  == 0b000)
#define CFG_MAME_BL4_MK_BUTTON1 (((config[12 + 2] >> 3) & 0b00000111)  == 0b001)
#define CFG_MAME_BL4_MK_BUTTON2 (((config[12 + 2] >> 3) & 0b00000111)  == 0b010)
#define CFG_MAME_BL4_MK_BUTTON3 (((config[12 + 2] >> 3) & 0b00000111)  == 0b011)
#define CFG_MAME_BL4_MK_BUTTON4 (((config[12 + 2] >> 3) & 0b00000111)  == 0b100)
#define CFG_MAME_BL4_MK_BUTTON5 (((config[12 + 2] >> 3) & 0b00000111)  == 0b101)
#define CFG_MAME_BL4_MK_BUTTON6 (((config[12 + 2] >> 3) & 0b00000111)  == 0b110)
#define CFG_MAME_BL4_HK_NOTHING ((((config[12 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL4_HK_BUTTON1 ((((config[12 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b00))
#define CFG_MAME_BL4_HK_BUTTON2 ((((config[12 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL4_HK_BUTTON3 ((((config[12 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b01))
#define CFG_MAME_BL4_HK_BUTTON4 ((((config[12 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL4_HK_BUTTON5 ((((config[12 + 2] >> 6) & 0b00000001)  == 0b1) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b10))
#define CFG_MAME_BL4_HK_BUTTON6 ((((config[12 + 2] >> 6) & 0b00000001)  == 0b0) && (((config[13 + 2] >> 0) & 0b00000011)  == 0b11))
#define CFG_MAME_BL4_4K_NOTHING (((config[13 + 2] >> 2) & 0b00000111)  == 0b000)
#define CFG_MAME_BL4_4K_BUTTON1 (((config[13 + 2] >> 2) & 0b00000111)  == 0b001)
#define CFG_MAME_BL4_4K_BUTTON2 (((config[13 + 2] >> 2) & 0b00000111)  == 0b010)
#define CFG_MAME_BL4_4K_BUTTON3 (((config[13 + 2] >> 2) & 0b00000111)  == 0b011)
#define CFG_MAME_BL4_4K_BUTTON4 (((config[13 + 2] >> 2) & 0b00000111)  == 0b100)
#define CFG_MAME_BL4_4K_BUTTON5 (((config[13 + 2] >> 2) & 0b00000111)  == 0b101)
#define CFG_MAME_BL4_4K_BUTTON6 (((config[13 + 2] >> 2) & 0b00000111)  == 0b110)
#define CFG_MAME_BL4_LP_NOTHING ((((config[13 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL4_LP_BUTTON1 ((((config[13 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL4_LP_BUTTON2 ((((config[13 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL4_LP_BUTTON3 ((((config[13 + 2] >> 5) & 0b00000011)  == 0b11) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b0))
#define CFG_MAME_BL4_LP_BUTTON4 ((((config[13 + 2] >> 5) & 0b00000011)  == 0b00) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL4_LP_BUTTON5 ((((config[13 + 2] >> 5) & 0b00000011)  == 0b01) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL4_LP_BUTTON6 ((((config[13 + 2] >> 5) & 0b00000011)  == 0b10) && (((config[14 + 2] >> 0) & 0b00000001)  == 0b1))
#define CFG_MAME_BL4_MP_NOTHING (((config[14 + 2] >> 1) & 0b00000111)  == 0b000)
#define CFG_MAME_BL4_MP_BUTTON1 (((config[14 + 2] >> 1) & 0b00000111)  == 0b001)
#define CFG_MAME_BL4_MP_BUTTON2 (((config[14 + 2] >> 1) & 0b00000111)  == 0b010)
#define CFG_MAME_BL4_MP_BUTTON3 (((config[14 + 2] >> 1) & 0b00000111)  == 0b011)
#define CFG_MAME_BL4_MP_BUTTON4 (((config[14 + 2] >> 1) & 0b00000111)  == 0b100)
#define CFG_MAME_BL4_MP_BUTTON5 (((config[14 + 2] >> 1) & 0b00000111)  == 0b101)
#define CFG_MAME_BL4_MP_BUTTON6 (((config[14 + 2] >> 1) & 0b00000111)  == 0b110)
#define CFG_MAME_BL4_HP_NOTHING (((config[14 + 2] >> 4) & 0b00000111)  == 0b000)
#define CFG_MAME_BL4_HP_BUTTON1 (((config[14 + 2] >> 4) & 0b00000111)  == 0b001)
#define CFG_MAME_BL4_HP_BUTTON2 (((config[14 + 2] >> 4) & 0b00000111)  == 0b010)
#define CFG_MAME_BL4_HP_BUTTON3 (((config[14 + 2] >> 4) & 0b00000111)  == 0b011)
#define CFG_MAME_BL4_HP_BUTTON4 (((config[14 + 2] >> 4) & 0b00000111)  == 0b100)
#define CFG_MAME_BL4_HP_BUTTON5 (((config[14 + 2] >> 4) & 0b00000111)  == 0b101)
#define CFG_MAME_BL4_HP_BUTTON6 (((config[14 + 2] >> 4) & 0b00000111)  == 0b110)
#define CFG_MAME_BL4_4P_NOTHING (((config[15 + 2] >> 0) & 0b00000111)  == 0b000)
#define CFG_MAME_BL4_4P_BUTTON1 (((config[15 + 2] >> 0) & 0b00000111)  == 0b001)
#define CFG_MAME_BL4_4P_BUTTON2 (((config[15 + 2] >> 0) & 0b00000111)  == 0b010)
#define CFG_MAME_BL4_4P_BUTTON3 (((config[15 + 2] >> 0) & 0b00000111)  == 0b011)
#define CFG_MAME_BL4_4P_BUTTON4 (((config[15 + 2] >> 0) & 0b00000111)  == 0b100)
#define CFG_MAME_BL4_4P_BUTTON5 (((config[15 + 2] >> 0) & 0b00000111)  == 0b101)
#define CFG_MAME_BL4_4P_BUTTON6 (((config[15 + 2] >> 0) & 0b00000111)  == 0b110)

/* CONFIG SETTERS: */
#define CFG_SET_DEF_WORK_MODE_PS3(CONFIG) CONFIG[0 + 2] &= ~(0b11 << 0);
#define CFG_SET_DEF_WORK_MODE_MAME(CONFIG) CONFIG[0 + 2] |= (0b01 << 0); CONFIG[0 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_DEF_WORK_MODE_XBOX(CONFIG) CONFIG[0 + 2] |= (0b10 << 0); CONFIG[0 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_DEF_WORK_MODE_PT(CONFIG) CONFIG[0 + 2] |= (0b11 << 0);
#define CFG_ENABLE_WORK_MODE_PS3_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<2);
#define CFG_DISABLE_WORK_MODE_PS3_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<2);
#define CFG_ENABLE_WORK_MODE_MAME_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<3);
#define CFG_DISABLE_WORK_MODE_MAME_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<3);
#define CFG_ENABLE_WORK_MODE_XBOX_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<4);
#define CFG_DISABLE_WORK_MODE_XBOX_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<4);
#define CFG_ENABLE_WORK_MODE_PT_ENABLED(CONFIG) CONFIG[0 + 2] |= (1<<5);
#define CFG_DISABLE_WORK_MODE_PT_ENABLED(CONFIG) CONFIG[0 + 2] &= ~(1<<5);
#define CFG_SET_DIGITAL_PAD(CONFIG) CONFIG[0 + 2] &= ~(0b1 << 6); CONFIG[1 + 2] &= ~(0b1 << 0);
#define CFG_SET_LEFT_STICK(CONFIG) CONFIG[0 + 2] |= (0b1 << 6); CONFIG[1 + 2] &= ~(0b1 << 0);
#define CFG_SET_RIGHT_STICK(CONFIG) CONFIG[0 + 2] &= ~(0b1 << 6); CONFIG[1 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_DEFAULT_PLAYER_1(CONFIG) CONFIG[1 + 2] &= ~(0b1 << 1);
#define CFG_SET_MAME_DEFAULT_PLAYER_2(CONFIG) CONFIG[1 + 2] |= (0b1 << 1);
#define CFG_ENABLE_HOME_EMU(CONFIG) CONFIG[1 + 2] |= (1<<2);
#define CFG_DISABLE_HOME_EMU(CONFIG) CONFIG[1 + 2] &= ~(1<<2);
#define CFG_SET_NO_EXTRA_PINS(CONFIG) CONFIG[1 + 2] &= ~(0b11 << 3);
#define CFG_SET_JOYSTICK_SWITCH_READ(CONFIG) CONFIG[1 + 2] |= (0b01 << 3); CONFIG[1 + 2] &= ((0b01 << 3)| ~(0b10 << 3));
#define CFG_SET_JOYSTICK_SWITCH_EMU(CONFIG) CONFIG[1 + 2] |= (0b10 << 3); CONFIG[1 + 2] &= ((0b10 << 3)| ~(0b01 << 3));
#define CFG_SET_INVERTED_TRIGGERS(CONFIG) CONFIG[1 + 2] |= (0b11 << 3);
#define CFG_SET_MAME_BL1_LK_NOTHING(CONFIG) CONFIG[1 + 2] &= ~(0b11 << 5); CONFIG[2 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_LK_BUTTON1(CONFIG) CONFIG[1 + 2] |= (0b01 << 5); CONFIG[1 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[2 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_LK_BUTTON2(CONFIG) CONFIG[1 + 2] |= (0b10 << 5); CONFIG[1 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[2 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_LK_BUTTON3(CONFIG) CONFIG[1 + 2] |= (0b11 << 5); CONFIG[2 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_LK_BUTTON4(CONFIG) CONFIG[1 + 2] &= ~(0b11 << 5); CONFIG[2 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL1_LK_BUTTON5(CONFIG) CONFIG[1 + 2] |= (0b01 << 5); CONFIG[1 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[2 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL1_LK_BUTTON6(CONFIG) CONFIG[1 + 2] |= (0b10 << 5); CONFIG[1 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[2 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL1_MK_NOTHING(CONFIG) CONFIG[2 + 2] &= ~(0b111 << 1);
#define CFG_SET_MAME_BL1_MK_BUTTON1(CONFIG) CONFIG[2 + 2] |= (0b001 << 1); CONFIG[2 + 2] &= ((0b001 << 1)| ~(0b110 << 1));
#define CFG_SET_MAME_BL1_MK_BUTTON2(CONFIG) CONFIG[2 + 2] |= (0b010 << 1); CONFIG[2 + 2] &= ((0b010 << 1)| ~(0b101 << 1));
#define CFG_SET_MAME_BL1_MK_BUTTON3(CONFIG) CONFIG[2 + 2] |= (0b011 << 1); CONFIG[2 + 2] &= ((0b011 << 1)| ~(0b100 << 1));
#define CFG_SET_MAME_BL1_MK_BUTTON4(CONFIG) CONFIG[2 + 2] |= (0b100 << 1); CONFIG[2 + 2] &= ((0b100 << 1)| ~(0b011 << 1));
#define CFG_SET_MAME_BL1_MK_BUTTON5(CONFIG) CONFIG[2 + 2] |= (0b101 << 1); CONFIG[2 + 2] &= ((0b101 << 1)| ~(0b010 << 1));
#define CFG_SET_MAME_BL1_MK_BUTTON6(CONFIG) CONFIG[2 + 2] |= (0b110 << 1); CONFIG[2 + 2] &= ((0b110 << 1)| ~(0b001 << 1));
#define CFG_SET_MAME_BL1_HK_NOTHING(CONFIG) CONFIG[2 + 2] &= ~(0b111 << 4);
#define CFG_SET_MAME_BL1_HK_BUTTON1(CONFIG) CONFIG[2 + 2] |= (0b001 << 4); CONFIG[2 + 2] &= ((0b001 << 4)| ~(0b110 << 4));
#define CFG_SET_MAME_BL1_HK_BUTTON2(CONFIG) CONFIG[2 + 2] |= (0b010 << 4); CONFIG[2 + 2] &= ((0b010 << 4)| ~(0b101 << 4));
#define CFG_SET_MAME_BL1_HK_BUTTON3(CONFIG) CONFIG[2 + 2] |= (0b011 << 4); CONFIG[2 + 2] &= ((0b011 << 4)| ~(0b100 << 4));
#define CFG_SET_MAME_BL1_HK_BUTTON4(CONFIG) CONFIG[2 + 2] |= (0b100 << 4); CONFIG[2 + 2] &= ((0b100 << 4)| ~(0b011 << 4));
#define CFG_SET_MAME_BL1_HK_BUTTON5(CONFIG) CONFIG[2 + 2] |= (0b101 << 4); CONFIG[2 + 2] &= ((0b101 << 4)| ~(0b010 << 4));
#define CFG_SET_MAME_BL1_HK_BUTTON6(CONFIG) CONFIG[2 + 2] |= (0b110 << 4); CONFIG[2 + 2] &= ((0b110 << 4)| ~(0b001 << 4));
#define CFG_SET_MAME_BL1_4K_NOTHING(CONFIG) CONFIG[3 + 2] &= ~(0b111 << 0);
#define CFG_SET_MAME_BL1_4K_BUTTON1(CONFIG) CONFIG[3 + 2] |= (0b001 << 0); CONFIG[3 + 2] &= ((0b001 << 0)| ~(0b110 << 0));
#define CFG_SET_MAME_BL1_4K_BUTTON2(CONFIG) CONFIG[3 + 2] |= (0b010 << 0); CONFIG[3 + 2] &= ((0b010 << 0)| ~(0b101 << 0));
#define CFG_SET_MAME_BL1_4K_BUTTON3(CONFIG) CONFIG[3 + 2] |= (0b011 << 0); CONFIG[3 + 2] &= ((0b011 << 0)| ~(0b100 << 0));
#define CFG_SET_MAME_BL1_4K_BUTTON4(CONFIG) CONFIG[3 + 2] |= (0b100 << 0); CONFIG[3 + 2] &= ((0b100 << 0)| ~(0b011 << 0));
#define CFG_SET_MAME_BL1_4K_BUTTON5(CONFIG) CONFIG[3 + 2] |= (0b101 << 0); CONFIG[3 + 2] &= ((0b101 << 0)| ~(0b010 << 0));
#define CFG_SET_MAME_BL1_4K_BUTTON6(CONFIG) CONFIG[3 + 2] |= (0b110 << 0); CONFIG[3 + 2] &= ((0b110 << 0)| ~(0b001 << 0));
#define CFG_SET_MAME_BL1_LP_NOTHING(CONFIG) CONFIG[3 + 2] &= ~(0b111 << 3);
#define CFG_SET_MAME_BL1_LP_BUTTON1(CONFIG) CONFIG[3 + 2] |= (0b001 << 3); CONFIG[3 + 2] &= ((0b001 << 3)| ~(0b110 << 3));
#define CFG_SET_MAME_BL1_LP_BUTTON2(CONFIG) CONFIG[3 + 2] |= (0b010 << 3); CONFIG[3 + 2] &= ((0b010 << 3)| ~(0b101 << 3));
#define CFG_SET_MAME_BL1_LP_BUTTON3(CONFIG) CONFIG[3 + 2] |= (0b011 << 3); CONFIG[3 + 2] &= ((0b011 << 3)| ~(0b100 << 3));
#define CFG_SET_MAME_BL1_LP_BUTTON4(CONFIG) CONFIG[3 + 2] |= (0b100 << 3); CONFIG[3 + 2] &= ((0b100 << 3)| ~(0b011 << 3));
#define CFG_SET_MAME_BL1_LP_BUTTON5(CONFIG) CONFIG[3 + 2] |= (0b101 << 3); CONFIG[3 + 2] &= ((0b101 << 3)| ~(0b010 << 3));
#define CFG_SET_MAME_BL1_LP_BUTTON6(CONFIG) CONFIG[3 + 2] |= (0b110 << 3); CONFIG[3 + 2] &= ((0b110 << 3)| ~(0b001 << 3));
#define CFG_SET_MAME_BL1_MP_NOTHING(CONFIG) CONFIG[3 + 2] &= ~(0b1 << 6); CONFIG[4 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL1_MP_BUTTON1(CONFIG) CONFIG[3 + 2] |= (0b1 << 6); CONFIG[4 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL1_MP_BUTTON2(CONFIG) CONFIG[3 + 2] &= ~(0b1 << 6); CONFIG[4 + 2] |= (0b01 << 0); CONFIG[4 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL1_MP_BUTTON3(CONFIG) CONFIG[3 + 2] |= (0b1 << 6); CONFIG[4 + 2] |= (0b01 << 0); CONFIG[4 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL1_MP_BUTTON4(CONFIG) CONFIG[3 + 2] &= ~(0b1 << 6); CONFIG[4 + 2] |= (0b10 << 0); CONFIG[4 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL1_MP_BUTTON5(CONFIG) CONFIG[3 + 2] |= (0b1 << 6); CONFIG[4 + 2] |= (0b10 << 0); CONFIG[4 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL1_MP_BUTTON6(CONFIG) CONFIG[3 + 2] &= ~(0b1 << 6); CONFIG[4 + 2] |= (0b11 << 0);
#define CFG_SET_MAME_BL1_HP_NOTHING(CONFIG) CONFIG[4 + 2] &= ~(0b111 << 2);
#define CFG_SET_MAME_BL1_HP_BUTTON1(CONFIG) CONFIG[4 + 2] |= (0b001 << 2); CONFIG[4 + 2] &= ((0b001 << 2)| ~(0b110 << 2));
#define CFG_SET_MAME_BL1_HP_BUTTON2(CONFIG) CONFIG[4 + 2] |= (0b010 << 2); CONFIG[4 + 2] &= ((0b010 << 2)| ~(0b101 << 2));
#define CFG_SET_MAME_BL1_HP_BUTTON3(CONFIG) CONFIG[4 + 2] |= (0b011 << 2); CONFIG[4 + 2] &= ((0b011 << 2)| ~(0b100 << 2));
#define CFG_SET_MAME_BL1_HP_BUTTON4(CONFIG) CONFIG[4 + 2] |= (0b100 << 2); CONFIG[4 + 2] &= ((0b100 << 2)| ~(0b011 << 2));
#define CFG_SET_MAME_BL1_HP_BUTTON5(CONFIG) CONFIG[4 + 2] |= (0b101 << 2); CONFIG[4 + 2] &= ((0b101 << 2)| ~(0b010 << 2));
#define CFG_SET_MAME_BL1_HP_BUTTON6(CONFIG) CONFIG[4 + 2] |= (0b110 << 2); CONFIG[4 + 2] &= ((0b110 << 2)| ~(0b001 << 2));
#define CFG_SET_MAME_BL1_4P_NOTHING(CONFIG) CONFIG[4 + 2] &= ~(0b11 << 5); CONFIG[5 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_4P_BUTTON1(CONFIG) CONFIG[4 + 2] |= (0b01 << 5); CONFIG[4 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[5 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_4P_BUTTON2(CONFIG) CONFIG[4 + 2] |= (0b10 << 5); CONFIG[4 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[5 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_4P_BUTTON3(CONFIG) CONFIG[4 + 2] |= (0b11 << 5); CONFIG[5 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL1_4P_BUTTON4(CONFIG) CONFIG[4 + 2] &= ~(0b11 << 5); CONFIG[5 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL1_4P_BUTTON5(CONFIG) CONFIG[4 + 2] |= (0b01 << 5); CONFIG[4 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[5 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL1_4P_BUTTON6(CONFIG) CONFIG[4 + 2] |= (0b10 << 5); CONFIG[4 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[5 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL2_LK_NOTHING(CONFIG) CONFIG[5 + 2] &= ~(0b111 << 1);
#define CFG_SET_MAME_BL2_LK_BUTTON1(CONFIG) CONFIG[5 + 2] |= (0b001 << 1); CONFIG[5 + 2] &= ((0b001 << 1)| ~(0b110 << 1));
#define CFG_SET_MAME_BL2_LK_BUTTON2(CONFIG) CONFIG[5 + 2] |= (0b010 << 1); CONFIG[5 + 2] &= ((0b010 << 1)| ~(0b101 << 1));
#define CFG_SET_MAME_BL2_LK_BUTTON3(CONFIG) CONFIG[5 + 2] |= (0b011 << 1); CONFIG[5 + 2] &= ((0b011 << 1)| ~(0b100 << 1));
#define CFG_SET_MAME_BL2_LK_BUTTON4(CONFIG) CONFIG[5 + 2] |= (0b100 << 1); CONFIG[5 + 2] &= ((0b100 << 1)| ~(0b011 << 1));
#define CFG_SET_MAME_BL2_LK_BUTTON5(CONFIG) CONFIG[5 + 2] |= (0b101 << 1); CONFIG[5 + 2] &= ((0b101 << 1)| ~(0b010 << 1));
#define CFG_SET_MAME_BL2_LK_BUTTON6(CONFIG) CONFIG[5 + 2] |= (0b110 << 1); CONFIG[5 + 2] &= ((0b110 << 1)| ~(0b001 << 1));
#define CFG_SET_MAME_BL2_MK_NOTHING(CONFIG) CONFIG[5 + 2] &= ~(0b111 << 4);
#define CFG_SET_MAME_BL2_MK_BUTTON1(CONFIG) CONFIG[5 + 2] |= (0b001 << 4); CONFIG[5 + 2] &= ((0b001 << 4)| ~(0b110 << 4));
#define CFG_SET_MAME_BL2_MK_BUTTON2(CONFIG) CONFIG[5 + 2] |= (0b010 << 4); CONFIG[5 + 2] &= ((0b010 << 4)| ~(0b101 << 4));
#define CFG_SET_MAME_BL2_MK_BUTTON3(CONFIG) CONFIG[5 + 2] |= (0b011 << 4); CONFIG[5 + 2] &= ((0b011 << 4)| ~(0b100 << 4));
#define CFG_SET_MAME_BL2_MK_BUTTON4(CONFIG) CONFIG[5 + 2] |= (0b100 << 4); CONFIG[5 + 2] &= ((0b100 << 4)| ~(0b011 << 4));
#define CFG_SET_MAME_BL2_MK_BUTTON5(CONFIG) CONFIG[5 + 2] |= (0b101 << 4); CONFIG[5 + 2] &= ((0b101 << 4)| ~(0b010 << 4));
#define CFG_SET_MAME_BL2_MK_BUTTON6(CONFIG) CONFIG[5 + 2] |= (0b110 << 4); CONFIG[5 + 2] &= ((0b110 << 4)| ~(0b001 << 4));
#define CFG_SET_MAME_BL2_HK_NOTHING(CONFIG) CONFIG[6 + 2] &= ~(0b111 << 0);
#define CFG_SET_MAME_BL2_HK_BUTTON1(CONFIG) CONFIG[6 + 2] |= (0b001 << 0); CONFIG[6 + 2] &= ((0b001 << 0)| ~(0b110 << 0));
#define CFG_SET_MAME_BL2_HK_BUTTON2(CONFIG) CONFIG[6 + 2] |= (0b010 << 0); CONFIG[6 + 2] &= ((0b010 << 0)| ~(0b101 << 0));
#define CFG_SET_MAME_BL2_HK_BUTTON3(CONFIG) CONFIG[6 + 2] |= (0b011 << 0); CONFIG[6 + 2] &= ((0b011 << 0)| ~(0b100 << 0));
#define CFG_SET_MAME_BL2_HK_BUTTON4(CONFIG) CONFIG[6 + 2] |= (0b100 << 0); CONFIG[6 + 2] &= ((0b100 << 0)| ~(0b011 << 0));
#define CFG_SET_MAME_BL2_HK_BUTTON5(CONFIG) CONFIG[6 + 2] |= (0b101 << 0); CONFIG[6 + 2] &= ((0b101 << 0)| ~(0b010 << 0));
#define CFG_SET_MAME_BL2_HK_BUTTON6(CONFIG) CONFIG[6 + 2] |= (0b110 << 0); CONFIG[6 + 2] &= ((0b110 << 0)| ~(0b001 << 0));
#define CFG_SET_MAME_BL2_4K_NOTHING(CONFIG) CONFIG[6 + 2] &= ~(0b111 << 3);
#define CFG_SET_MAME_BL2_4K_BUTTON1(CONFIG) CONFIG[6 + 2] |= (0b001 << 3); CONFIG[6 + 2] &= ((0b001 << 3)| ~(0b110 << 3));
#define CFG_SET_MAME_BL2_4K_BUTTON2(CONFIG) CONFIG[6 + 2] |= (0b010 << 3); CONFIG[6 + 2] &= ((0b010 << 3)| ~(0b101 << 3));
#define CFG_SET_MAME_BL2_4K_BUTTON3(CONFIG) CONFIG[6 + 2] |= (0b011 << 3); CONFIG[6 + 2] &= ((0b011 << 3)| ~(0b100 << 3));
#define CFG_SET_MAME_BL2_4K_BUTTON4(CONFIG) CONFIG[6 + 2] |= (0b100 << 3); CONFIG[6 + 2] &= ((0b100 << 3)| ~(0b011 << 3));
#define CFG_SET_MAME_BL2_4K_BUTTON5(CONFIG) CONFIG[6 + 2] |= (0b101 << 3); CONFIG[6 + 2] &= ((0b101 << 3)| ~(0b010 << 3));
#define CFG_SET_MAME_BL2_4K_BUTTON6(CONFIG) CONFIG[6 + 2] |= (0b110 << 3); CONFIG[6 + 2] &= ((0b110 << 3)| ~(0b001 << 3));
#define CFG_SET_MAME_BL2_LP_NOTHING(CONFIG) CONFIG[6 + 2] &= ~(0b1 << 6); CONFIG[7 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL2_LP_BUTTON1(CONFIG) CONFIG[6 + 2] |= (0b1 << 6); CONFIG[7 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL2_LP_BUTTON2(CONFIG) CONFIG[6 + 2] &= ~(0b1 << 6); CONFIG[7 + 2] |= (0b01 << 0); CONFIG[7 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL2_LP_BUTTON3(CONFIG) CONFIG[6 + 2] |= (0b1 << 6); CONFIG[7 + 2] |= (0b01 << 0); CONFIG[7 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL2_LP_BUTTON4(CONFIG) CONFIG[6 + 2] &= ~(0b1 << 6); CONFIG[7 + 2] |= (0b10 << 0); CONFIG[7 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL2_LP_BUTTON5(CONFIG) CONFIG[6 + 2] |= (0b1 << 6); CONFIG[7 + 2] |= (0b10 << 0); CONFIG[7 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL2_LP_BUTTON6(CONFIG) CONFIG[6 + 2] &= ~(0b1 << 6); CONFIG[7 + 2] |= (0b11 << 0);
#define CFG_SET_MAME_BL2_MP_NOTHING(CONFIG) CONFIG[7 + 2] &= ~(0b111 << 2);
#define CFG_SET_MAME_BL2_MP_BUTTON1(CONFIG) CONFIG[7 + 2] |= (0b001 << 2); CONFIG[7 + 2] &= ((0b001 << 2)| ~(0b110 << 2));
#define CFG_SET_MAME_BL2_MP_BUTTON2(CONFIG) CONFIG[7 + 2] |= (0b010 << 2); CONFIG[7 + 2] &= ((0b010 << 2)| ~(0b101 << 2));
#define CFG_SET_MAME_BL2_MP_BUTTON3(CONFIG) CONFIG[7 + 2] |= (0b011 << 2); CONFIG[7 + 2] &= ((0b011 << 2)| ~(0b100 << 2));
#define CFG_SET_MAME_BL2_MP_BUTTON4(CONFIG) CONFIG[7 + 2] |= (0b100 << 2); CONFIG[7 + 2] &= ((0b100 << 2)| ~(0b011 << 2));
#define CFG_SET_MAME_BL2_MP_BUTTON5(CONFIG) CONFIG[7 + 2] |= (0b101 << 2); CONFIG[7 + 2] &= ((0b101 << 2)| ~(0b010 << 2));
#define CFG_SET_MAME_BL2_MP_BUTTON6(CONFIG) CONFIG[7 + 2] |= (0b110 << 2); CONFIG[7 + 2] &= ((0b110 << 2)| ~(0b001 << 2));
#define CFG_SET_MAME_BL2_HP_NOTHING(CONFIG) CONFIG[7 + 2] &= ~(0b11 << 5); CONFIG[8 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL2_HP_BUTTON1(CONFIG) CONFIG[7 + 2] |= (0b01 << 5); CONFIG[7 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[8 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL2_HP_BUTTON2(CONFIG) CONFIG[7 + 2] |= (0b10 << 5); CONFIG[7 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[8 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL2_HP_BUTTON3(CONFIG) CONFIG[7 + 2] |= (0b11 << 5); CONFIG[8 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL2_HP_BUTTON4(CONFIG) CONFIG[7 + 2] &= ~(0b11 << 5); CONFIG[8 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL2_HP_BUTTON5(CONFIG) CONFIG[7 + 2] |= (0b01 << 5); CONFIG[7 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[8 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL2_HP_BUTTON6(CONFIG) CONFIG[7 + 2] |= (0b10 << 5); CONFIG[7 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[8 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL2_4P_NOTHING(CONFIG) CONFIG[8 + 2] &= ~(0b111 << 1);
#define CFG_SET_MAME_BL2_4P_BUTTON1(CONFIG) CONFIG[8 + 2] |= (0b001 << 1); CONFIG[8 + 2] &= ((0b001 << 1)| ~(0b110 << 1));
#define CFG_SET_MAME_BL2_4P_BUTTON2(CONFIG) CONFIG[8 + 2] |= (0b010 << 1); CONFIG[8 + 2] &= ((0b010 << 1)| ~(0b101 << 1));
#define CFG_SET_MAME_BL2_4P_BUTTON3(CONFIG) CONFIG[8 + 2] |= (0b011 << 1); CONFIG[8 + 2] &= ((0b011 << 1)| ~(0b100 << 1));
#define CFG_SET_MAME_BL2_4P_BUTTON4(CONFIG) CONFIG[8 + 2] |= (0b100 << 1); CONFIG[8 + 2] &= ((0b100 << 1)| ~(0b011 << 1));
#define CFG_SET_MAME_BL2_4P_BUTTON5(CONFIG) CONFIG[8 + 2] |= (0b101 << 1); CONFIG[8 + 2] &= ((0b101 << 1)| ~(0b010 << 1));
#define CFG_SET_MAME_BL2_4P_BUTTON6(CONFIG) CONFIG[8 + 2] |= (0b110 << 1); CONFIG[8 + 2] &= ((0b110 << 1)| ~(0b001 << 1));
#define CFG_SET_MAME_BL3_LK_NOTHING(CONFIG) CONFIG[8 + 2] &= ~(0b111 << 4);
#define CFG_SET_MAME_BL3_LK_BUTTON1(CONFIG) CONFIG[8 + 2] |= (0b001 << 4); CONFIG[8 + 2] &= ((0b001 << 4)| ~(0b110 << 4));
#define CFG_SET_MAME_BL3_LK_BUTTON2(CONFIG) CONFIG[8 + 2] |= (0b010 << 4); CONFIG[8 + 2] &= ((0b010 << 4)| ~(0b101 << 4));
#define CFG_SET_MAME_BL3_LK_BUTTON3(CONFIG) CONFIG[8 + 2] |= (0b011 << 4); CONFIG[8 + 2] &= ((0b011 << 4)| ~(0b100 << 4));
#define CFG_SET_MAME_BL3_LK_BUTTON4(CONFIG) CONFIG[8 + 2] |= (0b100 << 4); CONFIG[8 + 2] &= ((0b100 << 4)| ~(0b011 << 4));
#define CFG_SET_MAME_BL3_LK_BUTTON5(CONFIG) CONFIG[8 + 2] |= (0b101 << 4); CONFIG[8 + 2] &= ((0b101 << 4)| ~(0b010 << 4));
#define CFG_SET_MAME_BL3_LK_BUTTON6(CONFIG) CONFIG[8 + 2] |= (0b110 << 4); CONFIG[8 + 2] &= ((0b110 << 4)| ~(0b001 << 4));
#define CFG_SET_MAME_BL3_MK_NOTHING(CONFIG) CONFIG[9 + 2] &= ~(0b111 << 0);
#define CFG_SET_MAME_BL3_MK_BUTTON1(CONFIG) CONFIG[9 + 2] |= (0b001 << 0); CONFIG[9 + 2] &= ((0b001 << 0)| ~(0b110 << 0));
#define CFG_SET_MAME_BL3_MK_BUTTON2(CONFIG) CONFIG[9 + 2] |= (0b010 << 0); CONFIG[9 + 2] &= ((0b010 << 0)| ~(0b101 << 0));
#define CFG_SET_MAME_BL3_MK_BUTTON3(CONFIG) CONFIG[9 + 2] |= (0b011 << 0); CONFIG[9 + 2] &= ((0b011 << 0)| ~(0b100 << 0));
#define CFG_SET_MAME_BL3_MK_BUTTON4(CONFIG) CONFIG[9 + 2] |= (0b100 << 0); CONFIG[9 + 2] &= ((0b100 << 0)| ~(0b011 << 0));
#define CFG_SET_MAME_BL3_MK_BUTTON5(CONFIG) CONFIG[9 + 2] |= (0b101 << 0); CONFIG[9 + 2] &= ((0b101 << 0)| ~(0b010 << 0));
#define CFG_SET_MAME_BL3_MK_BUTTON6(CONFIG) CONFIG[9 + 2] |= (0b110 << 0); CONFIG[9 + 2] &= ((0b110 << 0)| ~(0b001 << 0));
#define CFG_SET_MAME_BL3_HK_NOTHING(CONFIG) CONFIG[9 + 2] &= ~(0b111 << 3);
#define CFG_SET_MAME_BL3_HK_BUTTON1(CONFIG) CONFIG[9 + 2] |= (0b001 << 3); CONFIG[9 + 2] &= ((0b001 << 3)| ~(0b110 << 3));
#define CFG_SET_MAME_BL3_HK_BUTTON2(CONFIG) CONFIG[9 + 2] |= (0b010 << 3); CONFIG[9 + 2] &= ((0b010 << 3)| ~(0b101 << 3));
#define CFG_SET_MAME_BL3_HK_BUTTON3(CONFIG) CONFIG[9 + 2] |= (0b011 << 3); CONFIG[9 + 2] &= ((0b011 << 3)| ~(0b100 << 3));
#define CFG_SET_MAME_BL3_HK_BUTTON4(CONFIG) CONFIG[9 + 2] |= (0b100 << 3); CONFIG[9 + 2] &= ((0b100 << 3)| ~(0b011 << 3));
#define CFG_SET_MAME_BL3_HK_BUTTON5(CONFIG) CONFIG[9 + 2] |= (0b101 << 3); CONFIG[9 + 2] &= ((0b101 << 3)| ~(0b010 << 3));
#define CFG_SET_MAME_BL3_HK_BUTTON6(CONFIG) CONFIG[9 + 2] |= (0b110 << 3); CONFIG[9 + 2] &= ((0b110 << 3)| ~(0b001 << 3));
#define CFG_SET_MAME_BL3_4K_NOTHING(CONFIG) CONFIG[9 + 2] &= ~(0b1 << 6); CONFIG[10 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL3_4K_BUTTON1(CONFIG) CONFIG[9 + 2] |= (0b1 << 6); CONFIG[10 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL3_4K_BUTTON2(CONFIG) CONFIG[9 + 2] &= ~(0b1 << 6); CONFIG[10 + 2] |= (0b01 << 0); CONFIG[10 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL3_4K_BUTTON3(CONFIG) CONFIG[9 + 2] |= (0b1 << 6); CONFIG[10 + 2] |= (0b01 << 0); CONFIG[10 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL3_4K_BUTTON4(CONFIG) CONFIG[9 + 2] &= ~(0b1 << 6); CONFIG[10 + 2] |= (0b10 << 0); CONFIG[10 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL3_4K_BUTTON5(CONFIG) CONFIG[9 + 2] |= (0b1 << 6); CONFIG[10 + 2] |= (0b10 << 0); CONFIG[10 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL3_4K_BUTTON6(CONFIG) CONFIG[9 + 2] &= ~(0b1 << 6); CONFIG[10 + 2] |= (0b11 << 0);
#define CFG_SET_MAME_BL3_LP_NOTHING(CONFIG) CONFIG[10 + 2] &= ~(0b111 << 2);
#define CFG_SET_MAME_BL3_LP_BUTTON1(CONFIG) CONFIG[10 + 2] |= (0b001 << 2); CONFIG[10 + 2] &= ((0b001 << 2)| ~(0b110 << 2));
#define CFG_SET_MAME_BL3_LP_BUTTON2(CONFIG) CONFIG[10 + 2] |= (0b010 << 2); CONFIG[10 + 2] &= ((0b010 << 2)| ~(0b101 << 2));
#define CFG_SET_MAME_BL3_LP_BUTTON3(CONFIG) CONFIG[10 + 2] |= (0b011 << 2); CONFIG[10 + 2] &= ((0b011 << 2)| ~(0b100 << 2));
#define CFG_SET_MAME_BL3_LP_BUTTON4(CONFIG) CONFIG[10 + 2] |= (0b100 << 2); CONFIG[10 + 2] &= ((0b100 << 2)| ~(0b011 << 2));
#define CFG_SET_MAME_BL3_LP_BUTTON5(CONFIG) CONFIG[10 + 2] |= (0b101 << 2); CONFIG[10 + 2] &= ((0b101 << 2)| ~(0b010 << 2));
#define CFG_SET_MAME_BL3_LP_BUTTON6(CONFIG) CONFIG[10 + 2] |= (0b110 << 2); CONFIG[10 + 2] &= ((0b110 << 2)| ~(0b001 << 2));
#define CFG_SET_MAME_BL3_MP_NOTHING(CONFIG) CONFIG[10 + 2] &= ~(0b11 << 5); CONFIG[11 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL3_MP_BUTTON1(CONFIG) CONFIG[10 + 2] |= (0b01 << 5); CONFIG[10 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[11 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL3_MP_BUTTON2(CONFIG) CONFIG[10 + 2] |= (0b10 << 5); CONFIG[10 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[11 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL3_MP_BUTTON3(CONFIG) CONFIG[10 + 2] |= (0b11 << 5); CONFIG[11 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL3_MP_BUTTON4(CONFIG) CONFIG[10 + 2] &= ~(0b11 << 5); CONFIG[11 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL3_MP_BUTTON5(CONFIG) CONFIG[10 + 2] |= (0b01 << 5); CONFIG[10 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[11 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL3_MP_BUTTON6(CONFIG) CONFIG[10 + 2] |= (0b10 << 5); CONFIG[10 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[11 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL3_HP_NOTHING(CONFIG) CONFIG[11 + 2] &= ~(0b111 << 1);
#define CFG_SET_MAME_BL3_HP_BUTTON1(CONFIG) CONFIG[11 + 2] |= (0b001 << 1); CONFIG[11 + 2] &= ((0b001 << 1)| ~(0b110 << 1));
#define CFG_SET_MAME_BL3_HP_BUTTON2(CONFIG) CONFIG[11 + 2] |= (0b010 << 1); CONFIG[11 + 2] &= ((0b010 << 1)| ~(0b101 << 1));
#define CFG_SET_MAME_BL3_HP_BUTTON3(CONFIG) CONFIG[11 + 2] |= (0b011 << 1); CONFIG[11 + 2] &= ((0b011 << 1)| ~(0b100 << 1));
#define CFG_SET_MAME_BL3_HP_BUTTON4(CONFIG) CONFIG[11 + 2] |= (0b100 << 1); CONFIG[11 + 2] &= ((0b100 << 1)| ~(0b011 << 1));
#define CFG_SET_MAME_BL3_HP_BUTTON5(CONFIG) CONFIG[11 + 2] |= (0b101 << 1); CONFIG[11 + 2] &= ((0b101 << 1)| ~(0b010 << 1));
#define CFG_SET_MAME_BL3_HP_BUTTON6(CONFIG) CONFIG[11 + 2] |= (0b110 << 1); CONFIG[11 + 2] &= ((0b110 << 1)| ~(0b001 << 1));
#define CFG_SET_MAME_BL3_4P_NOTHING(CONFIG) CONFIG[11 + 2] &= ~(0b111 << 4);
#define CFG_SET_MAME_BL3_4P_BUTTON1(CONFIG) CONFIG[11 + 2] |= (0b001 << 4); CONFIG[11 + 2] &= ((0b001 << 4)| ~(0b110 << 4));
#define CFG_SET_MAME_BL3_4P_BUTTON2(CONFIG) CONFIG[11 + 2] |= (0b010 << 4); CONFIG[11 + 2] &= ((0b010 << 4)| ~(0b101 << 4));
#define CFG_SET_MAME_BL3_4P_BUTTON3(CONFIG) CONFIG[11 + 2] |= (0b011 << 4); CONFIG[11 + 2] &= ((0b011 << 4)| ~(0b100 << 4));
#define CFG_SET_MAME_BL3_4P_BUTTON4(CONFIG) CONFIG[11 + 2] |= (0b100 << 4); CONFIG[11 + 2] &= ((0b100 << 4)| ~(0b011 << 4));
#define CFG_SET_MAME_BL3_4P_BUTTON5(CONFIG) CONFIG[11 + 2] |= (0b101 << 4); CONFIG[11 + 2] &= ((0b101 << 4)| ~(0b010 << 4));
#define CFG_SET_MAME_BL3_4P_BUTTON6(CONFIG) CONFIG[11 + 2] |= (0b110 << 4); CONFIG[11 + 2] &= ((0b110 << 4)| ~(0b001 << 4));
#define CFG_SET_MAME_BL4_LK_NOTHING(CONFIG) CONFIG[12 + 2] &= ~(0b111 << 0);
#define CFG_SET_MAME_BL4_LK_BUTTON1(CONFIG) CONFIG[12 + 2] |= (0b001 << 0); CONFIG[12 + 2] &= ((0b001 << 0)| ~(0b110 << 0));
#define CFG_SET_MAME_BL4_LK_BUTTON2(CONFIG) CONFIG[12 + 2] |= (0b010 << 0); CONFIG[12 + 2] &= ((0b010 << 0)| ~(0b101 << 0));
#define CFG_SET_MAME_BL4_LK_BUTTON3(CONFIG) CONFIG[12 + 2] |= (0b011 << 0); CONFIG[12 + 2] &= ((0b011 << 0)| ~(0b100 << 0));
#define CFG_SET_MAME_BL4_LK_BUTTON4(CONFIG) CONFIG[12 + 2] |= (0b100 << 0); CONFIG[12 + 2] &= ((0b100 << 0)| ~(0b011 << 0));
#define CFG_SET_MAME_BL4_LK_BUTTON5(CONFIG) CONFIG[12 + 2] |= (0b101 << 0); CONFIG[12 + 2] &= ((0b101 << 0)| ~(0b010 << 0));
#define CFG_SET_MAME_BL4_LK_BUTTON6(CONFIG) CONFIG[12 + 2] |= (0b110 << 0); CONFIG[12 + 2] &= ((0b110 << 0)| ~(0b001 << 0));
#define CFG_SET_MAME_BL4_MK_NOTHING(CONFIG) CONFIG[12 + 2] &= ~(0b111 << 3);
#define CFG_SET_MAME_BL4_MK_BUTTON1(CONFIG) CONFIG[12 + 2] |= (0b001 << 3); CONFIG[12 + 2] &= ((0b001 << 3)| ~(0b110 << 3));
#define CFG_SET_MAME_BL4_MK_BUTTON2(CONFIG) CONFIG[12 + 2] |= (0b010 << 3); CONFIG[12 + 2] &= ((0b010 << 3)| ~(0b101 << 3));
#define CFG_SET_MAME_BL4_MK_BUTTON3(CONFIG) CONFIG[12 + 2] |= (0b011 << 3); CONFIG[12 + 2] &= ((0b011 << 3)| ~(0b100 << 3));
#define CFG_SET_MAME_BL4_MK_BUTTON4(CONFIG) CONFIG[12 + 2] |= (0b100 << 3); CONFIG[12 + 2] &= ((0b100 << 3)| ~(0b011 << 3));
#define CFG_SET_MAME_BL4_MK_BUTTON5(CONFIG) CONFIG[12 + 2] |= (0b101 << 3); CONFIG[12 + 2] &= ((0b101 << 3)| ~(0b010 << 3));
#define CFG_SET_MAME_BL4_MK_BUTTON6(CONFIG) CONFIG[12 + 2] |= (0b110 << 3); CONFIG[12 + 2] &= ((0b110 << 3)| ~(0b001 << 3));
#define CFG_SET_MAME_BL4_HK_NOTHING(CONFIG) CONFIG[12 + 2] &= ~(0b1 << 6); CONFIG[13 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL4_HK_BUTTON1(CONFIG) CONFIG[12 + 2] |= (0b1 << 6); CONFIG[13 + 2] &= ~(0b11 << 0);
#define CFG_SET_MAME_BL4_HK_BUTTON2(CONFIG) CONFIG[12 + 2] &= ~(0b1 << 6); CONFIG[13 + 2] |= (0b01 << 0); CONFIG[13 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL4_HK_BUTTON3(CONFIG) CONFIG[12 + 2] |= (0b1 << 6); CONFIG[13 + 2] |= (0b01 << 0); CONFIG[13 + 2] &= ((0b01 << 0)| ~(0b10 << 0));
#define CFG_SET_MAME_BL4_HK_BUTTON4(CONFIG) CONFIG[12 + 2] &= ~(0b1 << 6); CONFIG[13 + 2] |= (0b10 << 0); CONFIG[13 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL4_HK_BUTTON5(CONFIG) CONFIG[12 + 2] |= (0b1 << 6); CONFIG[13 + 2] |= (0b10 << 0); CONFIG[13 + 2] &= ((0b10 << 0)| ~(0b01 << 0));
#define CFG_SET_MAME_BL4_HK_BUTTON6(CONFIG) CONFIG[12 + 2] &= ~(0b1 << 6); CONFIG[13 + 2] |= (0b11 << 0);
#define CFG_SET_MAME_BL4_4K_NOTHING(CONFIG) CONFIG[13 + 2] &= ~(0b111 << 2);
#define CFG_SET_MAME_BL4_4K_BUTTON1(CONFIG) CONFIG[13 + 2] |= (0b001 << 2); CONFIG[13 + 2] &= ((0b001 << 2)| ~(0b110 << 2));
#define CFG_SET_MAME_BL4_4K_BUTTON2(CONFIG) CONFIG[13 + 2] |= (0b010 << 2); CONFIG[13 + 2] &= ((0b010 << 2)| ~(0b101 << 2));
#define CFG_SET_MAME_BL4_4K_BUTTON3(CONFIG) CONFIG[13 + 2] |= (0b011 << 2); CONFIG[13 + 2] &= ((0b011 << 2)| ~(0b100 << 2));
#define CFG_SET_MAME_BL4_4K_BUTTON4(CONFIG) CONFIG[13 + 2] |= (0b100 << 2); CONFIG[13 + 2] &= ((0b100 << 2)| ~(0b011 << 2));
#define CFG_SET_MAME_BL4_4K_BUTTON5(CONFIG) CONFIG[13 + 2] |= (0b101 << 2); CONFIG[13 + 2] &= ((0b101 << 2)| ~(0b010 << 2));
#define CFG_SET_MAME_BL4_4K_BUTTON6(CONFIG) CONFIG[13 + 2] |= (0b110 << 2); CONFIG[13 + 2] &= ((0b110 << 2)| ~(0b001 << 2));
#define CFG_SET_MAME_BL4_LP_NOTHING(CONFIG) CONFIG[13 + 2] &= ~(0b11 << 5); CONFIG[14 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL4_LP_BUTTON1(CONFIG) CONFIG[13 + 2] |= (0b01 << 5); CONFIG[13 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[14 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL4_LP_BUTTON2(CONFIG) CONFIG[13 + 2] |= (0b10 << 5); CONFIG[13 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[14 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL4_LP_BUTTON3(CONFIG) CONFIG[13 + 2] |= (0b11 << 5); CONFIG[14 + 2] &= ~(0b1 << 0);
#define CFG_SET_MAME_BL4_LP_BUTTON4(CONFIG) CONFIG[13 + 2] &= ~(0b11 << 5); CONFIG[14 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL4_LP_BUTTON5(CONFIG) CONFIG[13 + 2] |= (0b01 << 5); CONFIG[13 + 2] &= ((0b01 << 5)| ~(0b10 << 5)); CONFIG[14 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL4_LP_BUTTON6(CONFIG) CONFIG[13 + 2] |= (0b10 << 5); CONFIG[13 + 2] &= ((0b10 << 5)| ~(0b01 << 5)); CONFIG[14 + 2] |= (0b1 << 0);
#define CFG_SET_MAME_BL4_MP_NOTHING(CONFIG) CONFIG[14 + 2] &= ~(0b111 << 1);
#define CFG_SET_MAME_BL4_MP_BUTTON1(CONFIG) CONFIG[14 + 2] |= (0b001 << 1); CONFIG[14 + 2] &= ((0b001 << 1)| ~(0b110 << 1));
#define CFG_SET_MAME_BL4_MP_BUTTON2(CONFIG) CONFIG[14 + 2] |= (0b010 << 1); CONFIG[14 + 2] &= ((0b010 << 1)| ~(0b101 << 1));
#define CFG_SET_MAME_BL4_MP_BUTTON3(CONFIG) CONFIG[14 + 2] |= (0b011 << 1); CONFIG[14 + 2] &= ((0b011 << 1)| ~(0b100 << 1));
#define CFG_SET_MAME_BL4_MP_BUTTON4(CONFIG) CONFIG[14 + 2] |= (0b100 << 1); CONFIG[14 + 2] &= ((0b100 << 1)| ~(0b011 << 1));
#define CFG_SET_MAME_BL4_MP_BUTTON5(CONFIG) CONFIG[14 + 2] |= (0b101 << 1); CONFIG[14 + 2] &= ((0b101 << 1)| ~(0b010 << 1));
#define CFG_SET_MAME_BL4_MP_BUTTON6(CONFIG) CONFIG[14 + 2] |= (0b110 << 1); CONFIG[14 + 2] &= ((0b110 << 1)| ~(0b001 << 1));
#define CFG_SET_MAME_BL4_HP_NOTHING(CONFIG) CONFIG[14 + 2] &= ~(0b111 << 4);
#define CFG_SET_MAME_BL4_HP_BUTTON1(CONFIG) CONFIG[14 + 2] |= (0b001 << 4); CONFIG[14 + 2] &= ((0b001 << 4)| ~(0b110 << 4));
#define CFG_SET_MAME_BL4_HP_BUTTON2(CONFIG) CONFIG[14 + 2] |= (0b010 << 4); CONFIG[14 + 2] &= ((0b010 << 4)| ~(0b101 << 4));
#define CFG_SET_MAME_BL4_HP_BUTTON3(CONFIG) CONFIG[14 + 2] |= (0b011 << 4); CONFIG[14 + 2] &= ((0b011 << 4)| ~(0b100 << 4));
#define CFG_SET_MAME_BL4_HP_BUTTON4(CONFIG) CONFIG[14 + 2] |= (0b100 << 4); CONFIG[14 + 2] &= ((0b100 << 4)| ~(0b011 << 4));
#define CFG_SET_MAME_BL4_HP_BUTTON5(CONFIG) CONFIG[14 + 2] |= (0b101 << 4); CONFIG[14 + 2] &= ((0b101 << 4)| ~(0b010 << 4));
#define CFG_SET_MAME_BL4_HP_BUTTON6(CONFIG) CONFIG[14 + 2] |= (0b110 << 4); CONFIG[14 + 2] &= ((0b110 << 4)| ~(0b001 << 4));
#define CFG_SET_MAME_BL4_4P_NOTHING(CONFIG) CONFIG[15 + 2] &= ~(0b111 << 0);
#define CFG_SET_MAME_BL4_4P_BUTTON1(CONFIG) CONFIG[15 + 2] |= (0b001 << 0); CONFIG[15 + 2] &= ((0b001 << 0)| ~(0b110 << 0));
#define CFG_SET_MAME_BL4_4P_BUTTON2(CONFIG) CONFIG[15 + 2] |= (0b010 << 0); CONFIG[15 + 2] &= ((0b010 << 0)| ~(0b101 << 0));
#define CFG_SET_MAME_BL4_4P_BUTTON3(CONFIG) CONFIG[15 + 2] |= (0b011 << 0); CONFIG[15 + 2] &= ((0b011 << 0)| ~(0b100 << 0));
#define CFG_SET_MAME_BL4_4P_BUTTON4(CONFIG) CONFIG[15 + 2] |= (0b100 << 0); CONFIG[15 + 2] &= ((0b100 << 0)| ~(0b011 << 0));
#define CFG_SET_MAME_BL4_4P_BUTTON5(CONFIG) CONFIG[15 + 2] |= (0b101 << 0); CONFIG[15 + 2] &= ((0b101 << 0)| ~(0b010 << 0));
#define CFG_SET_MAME_BL4_4P_BUTTON6(CONFIG) CONFIG[15 + 2] |= (0b110 << 0); CONFIG[15 + 2] &= ((0b110 << 0)| ~(0b001 << 0));

#endif
