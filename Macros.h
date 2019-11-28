#pragma once
#include <lcom/lcf.h>

//IRQ
#define KBD_IRQ 1
#define TIMER0_IRQ 0
#define MOUSE_IRQ 12


//--Mouse--
//Commands
#define SET_STRAM_MODE 0xEA
#define ENABLE_DATA_REPORTING 0xF4
#define WRITE_BYTE_TO_MOUSE 0xD4
#define DISABLE_DATA_REPORTING 0xF5

//Acknowledgment byte
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

//Regs
#define KBC_CMD_REG 0x64
#define STAT_REG 0x64
#define OUT_BUF 0x60

//Flags
#define OBF BIT(0) //Full if 1 (data available for reading)
#define IBF_FULL BIT(1)
#define MOUSE BIT(5) //Data is from mouse
#define TO_ERR BIT(6) //Timeout error if 1
#define PAR_ERR BIT(7) //Parity error if 1


//--KBD--
//Makecode/Breakcode
#define MB BIT(7) //1 - breakcode 0 - makecode
#define MSB_2B 0xE0
#define ESC_BRKCD 0x81

//Timer
#define TIMER_0 0x40
#define TIMER_FREQ 1193182              
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB)
#define TIMER_CTRL 0x43
#define TIMER_LSB BIT(4)
#define TIMER_MSB BIT(5)     
#define FPS 60             


//Screen size
#define H_RES 800
#define V_RES 600
#define MODE 0x115

//XPM
#define XPM_BYTES_PER_PIXEL 3
#define SYSTEM_BYTES_PER_PIXEL 3
#define BITS_PER_PIXEL 24


//Other
#define DELAY_US 20000
