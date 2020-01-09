#pragma once
#include <lcom/lcf.h>

//IRQ
#define KBD_IRQ 1 /**< @brief Keyboard IRQ line */
#define TIMER0_IRQ 0 /**< @brief timer0 IRQ line */
#define MOUSE_IRQ 12 /**< @brief mouse IRQ line */
#define RTC_IRQ 8 /**< @brief rtc IRQ line */

//--RTC--
#define RTC_ADDR_REG 0x70 /**< @brief RTC Address Register*/
#define RTC_REG_A 10 /**< @brief Register A*/
#define RTC_REG_B 11 /**< @brief Register B*/ 
#define RTC_REG_C 12 /**< @brief Register C*/
#define RTC_DATA_REG 0x71 /**< @brief RTC Data Register*/
#define RTC_UIP BIT(7) /**< @brief Bit UIP of Register A */
#define SECONDS 0 /**< @brief RTC byte that contains current seconds */
#define READ_MIN 2 /**< @brief RTC byte that contains current minutes */
#define READ_HOURS 4 /**< @brief RTC byte that contains current hours */
#define ALARM_SEC 1 /**< @brief RTC byte that controls the seconds portion of the alarm */
#define ALARM_MIN 3 /**< @brief RTC byte that controls the minutes portion of the alarm */
#define ALARM_HOURS 5 /**< @brief RTC byte that controls the hours portion of the alarm */
#define ALARM_IGNORE_VALUE BIT(7) | BIT(6) /**< @brief don't care value for rtc alarms*/

//--Mouse--
//Commands
#define SET_STRAM_MODE 0xEA /**< @brief Command for the KBC to set stream mode*/
#define ENABLE_DATA_REPORTING 0xF4 /**< @brief Command for the KBC to enable data reporting*/
#define WRITE_BYTE_TO_MOUSE 0xD4 /**< @brief Command for the KBC to write byte to mouse*/
#define DISABLE_DATA_REPORTING 0xF5 /**< @brief Command for the KBC to disable data reporting*/

//Acknowledgment byte
#define ACK 0xFA /**< @brief Acknowledgment byte - everything ok*/
#define NACK 0xFE /**< @brief Acknowledgment byte - invalid byte*/
#define ERROR 0xFC /**< @brief Acknowledgment byte - second consecutive invalid byte*/

//Regs
#define KBC_CMD_REG 0x64 /**< @brief Commands to the KBC Register */
#define STAT_REG 0x64 /**< @brief Status Register */
#define OUT_BUF 0x60 /**< @brief Output buffer */

//Flags
#define OBF BIT(0) /**< @brief Out buffer is full if 1 */ 
#define IBF_FULL BIT(1) /**< @brief In buffer is full if 1 */
#define MOUSE BIT(5) /**< @brief Data is from mouse if 1 */ 
#define TO_ERR BIT(6) /**< @brief Timeout error if 1 */
#define PAR_ERR BIT(7) /**< @brief Parity error if 1 */


//--KBD--
#define MB BIT(7) /**< @brief 1 if it is breakcode. 0 if it is a makecode */ 
#define MSB_2B 0xE0 /**< @brief Most significant byte of 2 bytes scancodes */
#define ESC_BRKCD 0x81 /**< @brief Esc break code */
#define ENTER_MKCD 0x1C /**< @brief Enter make code */
#define BACKSPACE_MKCD 0x0e /**< @brief Backspace makecode  */

//Timer
#define TIMER_0 0x40 /**< @brief Timer 0 count register */
#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */       
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */
#define TIMER_CTRL 0x43 /**< @brief Control register*/
#define TIMER_LSB BIT(4) /**< @brief Initialize Counter LSB only */
#define TIMER_MSB BIT(5) /**< @brief Initialize Counter MSB only */
#define FPS 60 /**< @brief Frames per second */   

//Screen size
#define H_RES 800 /**< @brief Horizontal resolution */
#define V_RES 600 /**< @brief Vertical resolution */
#define MODE 0x115 /**< @brief Graphics mode to use*/

//XPM
#define XPM_BYTES_PER_PIXEL 3 /**< @brief Xpm bytes per pixel */
#define SYSTEM_BYTES_PER_PIXEL 3 /**< @brief System bytes per pixel */
#define BITS_PER_PIXEL 24 /**< @brief Bits per pixel */

//Coins
#define COIN_LIMIT 99 /**< @brief Limit of coins */

//ABILITIES
//Kills every enemy onscreen
#define COINS_FOR_NUKE 40 /**< @brief Cost of nuke powerup */

//Slows incoming and onscreen enemies for x seconds and lowers the spawn rate by y percent
#define COINS_FOR_ANTIVIRUS 20 /**< @brief Cost of antivirus powerup */
#define ANTIVIRUS_SLOW 33 /**< @brief Powerup slowdown of enemies */
#define ANTIVIRUS_SPAWN_REDUCTION 15 /**< @brief Powerup enemies spawn reduction */
#define ANTIVIRUS_DURATION 15        /**< @brief Antivirus duration */


//ENEMIES
#define TIME_BETWEEN_DIFFICULTIES 1800 /**< @brief Time between the change of difficulties */
#define BASE_ENEMY_SPAWN_RATE 150
#define ENEMY_SPAWN_RATE(cycles) (BASE_ENEMY_SPAWN_RATE - cycles * 4) / (((cycles%2 )/2.0) + 1) /**< @brief Enemies spawn rate */
#define ENEMY_LIMIT 20 /**< @brief Limit of enemies to be created */

//Missiles
#define Missile_Speed_1 8 /**< @brief 1st Missile speed */
#define Missile_Speed_2 50 /**< @brief 2nd Missile speed */
#define Missile_Speed_3 10 /**< @brief 3rd Missile speed */
#define Missile_Speed_4 25 /**< @brief 4th Missile speed */

//Other
#define DELAY_US 20000 /**< @brief Used as delay*/
