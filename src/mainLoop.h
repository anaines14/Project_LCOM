#pragma once
#include <lcom/lcf.h>
#include <lcom/liblm.h>

//XPMS
#include "./Xpms/Background.xpm"
#include "./Xpms/Cursor.xpm"
#include "./Xpms/Cursor_Left_Click.xpm"
#include "./Xpms/Dot.xpm"
#include "./Xpms/login.xpm"
#include "./Xpms/Menu.xpm"
#include "./Xpms/Missil1.xpm"
#include "./Xpms/Missil2.xpm"
#include "./Xpms/Missil3.xpm"
#include "./Xpms/Missil4.xpm"
#include "./Xpms/Missil5.xpm"
#include "./Xpms/Missil6.xpm"
#include "./Xpms/Missil7.xpm"
#include "./Xpms/Missil8.xpm"
#include "./Xpms/Missil9.xpm"
#include "./Xpms/Start_Button_Pressed.xpm"
#include "./Xpms/VirtualBox.xpm"
#include "./Xpms/Zero.xpm"
#include "./Xpms/Um.xpm"
#include "./Xpms/Dois.xpm"
#include "./Xpms/Tres.xpm"
#include "./Xpms/Quatro.xpm"
#include "./Xpms/Cinco.xpm"
#include "./Xpms/Seis.xpm"
#include "./Xpms/Sete.xpm"
#include "./Xpms/Oito.xpm"
#include "./Xpms/Nove.xpm"
#include "./Xpms/Dois_Pontos.xpm"
#include "./Xpms/BeetleUp1.xpm"
#include "./Xpms/BeetleUp2.xpm"
#include "./Xpms/BeetleUp3.xpm"
#include "./Xpms/BeetleUpR1.xpm"
#include "./Xpms/BeetleUpR2.xpm"
#include "./Xpms/BeetleUpR3.xpm"
#include "./Xpms/BeetleUpRight1.xpm"
#include "./Xpms/BeetleUpRight2.xpm"
#include "./Xpms/BeetleUpRight3.xpm"
#include "./Xpms/BeetleURight1.xpm"
#include "./Xpms/BeetleURight2.xpm"
#include "./Xpms/BeetleURight3.xpm"
#include "./Xpms/Coin1.xpm"
#include "./Xpms/Coin2.xpm"
#include "./Xpms/Coin3.xpm"
#include "./Xpms/Coin4.xpm"
#include "./Xpms/Coin5.xpm"
#include "./Xpms/Coin6.xpm"
#include "./Xpms/EndScreen.xpm"
#include "./Xpms/ZeroMoeda.xpm"
#include "./Xpms/UmMoeda.xpm"
#include "./Xpms/DoisMoeda.xpm"
#include "./Xpms/TresMoeda.xpm"
#include "./Xpms/QuatroMoeda.xpm"
#include "./Xpms/CincoMoeda.xpm"
#include "./Xpms/SeisMoeda.xpm"
#include "./Xpms/SeteMoeda.xpm"
#include "./Xpms/OitoMoeda.xpm"
#include "./Xpms/NoveMoeda.xpm"
#include "./Xpms/Cooldown_Bar.xpm"
#include "./Xpms/Nuke.xpm"
#include "./Xpms/Antivirus.xpm"
  //---

/**
 * @brief Unsubscribes all interruptions subscribed. Disables data reporting of mouse. Exits graphics mode.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int unsubscribeAll();
/**
 * @brief Subscribes all interruptions needed. Eables data reporting of mouse and sets stream mode. Setss graphics mode.
 * 
 * @param hook_id_kbd Hook id of keyboard
 * @param hook_id_timer0 Hook id of timer 0
 * @param hook_id_mouse Hook id of mouse
 * @param hook_id_rtc Hook id of rtc
 * @return Return 0 upon success and non-zero otherwise.
 */
int subscribeAll(uint8_t hook_id_kbd, uint8_t hook_id_timer0, uint8_t hook_id_mouse,  uint8_t hook_id_rtc);
/**
 * @brief Responsible for starting the game by calling the interrupts functions.
 * 
 * @return interrupts()
 */
int (game_start)();
/**
 * @brief Runs basically everything related to the game. Handles timer, keyboard, mouse and rtc interrupts. Mostly handles graphicsInitializes important variables, like the game timer. Spawns all of the sprites (enimies, coins, numbers for coins and clock, ability frames and icons, cursor, missiles, start menu sprites, regular menu and more)and the background. Spawns, moves, checks collisions, animates and destroys enimies. Makes the cursor follow the mouse and, when left clicking, spawns a missile in that position that destroys enimies on contact. Controls when the game is lost, which is when the enimies reach the virtual box icon in the middle. Controls activated abilities, which happen when pressing the z or x key if the player has enough coins. 
 */
 int (interrupts)();
