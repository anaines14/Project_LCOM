#include <lcom/lcf.h>
#include <lcom/liblm.h>

#include "VBE.h"
#include "animations.h"
#include "keyboard.h"
#include "mainLoop.h"
#include "mouse.h"
#include "rtc.h"
#include "sprites.h"
#include "timer.h"
#include "utils.h"

//GLOBAL KEYBOARD VARIABLES
extern uint8_t msB, lsB;
extern bool success;

//GLOBAL MOUSE VARIABLES
extern int read_result;
extern uint8_t bytes[];
extern struct packet pp;
extern uint8_t byteNumMouse;
extern uint32_t interrupt_counter;
extern struct mouse_ev ev;
extern uint16_t x_percorrido;

//Global screen variables
extern void *video_mem;

//Time variables
extern int minutes, hours;

int(game_start)() {
  return interrupts();
}

int(interrupts)() {
  //Counters
  unsigned int frames_counter = 0, cycles = 0;
  int dots_counter = 0;

  //Bools
  bool menu_open = false, loged_in = false, perder_jogo = false;

  //Mouse variables
  float true_x_mouse = H_RES;
  float true_y_mouse = V_RES;
  float x_mouse = (H_RES + 1) >> 1;
  float y_mouse = (V_RES + 1) >> 1;
  //Mouse clicks
  bool leftClickLastFrame = false; //Frame anterior
  bool leftClick = false;          //Frame atual

  //Other
  int ipc_status, r;
  message msg;

  //Keyboard
  uint8_t hook_id_kbd = KBD_IRQ;
  int irq_kbd = BIT(hook_id_kbd);
  uint8_t size;
  bool make = true, read_lsb = false;

  //Timer
  uint8_t hook_id_timer0 = TIMER0_IRQ;
  int irq_timer0 = BIT(hook_id_timer0);

  //Mouse
  uint8_t hook_id_mouse = MOUSE_IRQ;
  int irq_mouse = BIT(hook_id_mouse);

  //RTC
  uint8_t hook_id_rtc = RTC_IRQ;
  int irq_rtc = BIT(hook_id_rtc);

  subscribeAll(hook_id_kbd, hook_id_timer0, hook_id_mouse, hook_id_rtc);

  //Creating a memory buffer
  char *memBuffer = (char *) malloc(H_RES * V_RES * SYSTEM_BYTES_PER_PIXEL * sizeof(uint8_t));

  //ABILITIES
  uint16_t antivirus_counter = 0;
  bool antivirus_enabled = false;

  //CREATING SPRITES
  Sprite *lg = create_sprite(login, 0, 0, 0, 0);
  Sprite *bg = create_sprite(background, 0, 0, 0, 0);
  Sprite *vBox = create_sprite(Virtual_Box, 387, 283, 0, 0);
  Sprite *menubar = create_sprite(Menu, 0, 93, 0, 0);
  Sprite *startpressed = create_sprite(Start_Button_Pressed, 0, 570, 0, 0);
  Sprite *dt = create_sprite(Dot, 300, 405, 0, 0);
  Sprite *endScreen = create_sprite(EndScreen, (H_RES - 269) >> 1, (V_RES - 120) >> 1, 0, 0);

  //---ENEMIES-------------------
  //Creating an Animated Enemy
  const char **AnimatedArr[4];
  AnimatedArr[0] = BeetleUp1;
  AnimatedArr[1] = BeetleUp2;
  AnimatedArr[2] = BeetleUp1;
  AnimatedArr[3] = BeetleUp3;

  const char **Beetle_2[4];
  Beetle_2[0] = BeetleUpR1;
  Beetle_2[1] = BeetleUpR2;
  Beetle_2[2] = BeetleUpR1;
  Beetle_2[3] = BeetleUpR3;

  const char **Beetle_3[4];
  Beetle_3[0] = BeetleUpRight1;
  Beetle_3[1] = BeetleUpRight2;
  Beetle_3[2] = BeetleUpRight1;
  Beetle_3[3] = BeetleUpRight3;

  const char **Beetle_4[4];
  Beetle_4[0] = BeetleURight1;
  Beetle_4[1] = BeetleURight2;
  Beetle_4[2] = BeetleURight1;
  Beetle_4[3] = BeetleURight3;

  //Creating all enemies
  Animated *enemies[ENEMY_LIMIT];
  for (int i = 0; i < ENEMY_LIMIT; i++)
    enemies[i] = NULL;

  //-----COINS---------------
  const char **Coins[8];
  Coins[0] = Coin1;
  Coins[1] = Coin2;
  Coins[2] = Coin3;
  Coins[3] = Coin4;
  Coins[4] = Coin4;
  Coins[5] = Coin5;
  Coins[6] = Coin6;
  Coins[7] = Coin1;
  Animated *Coin = setAnimated(676, 577, 0, 0, 10, Coins, 8);
  uint8_t coins = 0; //Max 99

  //Cooldown Bars
  Sprite *ability_bars[2];
  for (int i = 0; i < 2; i++)
    ability_bars[i] = create_sprite(Cooldown_Bar, 132 + 100 * i, 578, 0, 0);

  Sprite *nuke = create_sprite(Nuke, 108, 578, 0, 0);
  Sprite *antivirus = create_sprite(Antivirus, 208, 578, 0, 0);

  int8_t nuke_cd = 90;

  //-----NUMBERS---------
  const char **number[10];
  number[0] = ZeroMoeda;
  number[1] = UmMoeda;
  number[2] = DoisMoeda;
  number[3] = TresMoeda;
  number[4] = QuatroMoeda;
  number[5] = CincoMoeda;
  number[6] = SeisMoeda;
  number[7] = SeteMoeda;
  number[8] = OitoMoeda;
  number[9] = NoveMoeda;

  Animated *numbers[2];
  for (int i = 0; i < 2; i++)
    numbers[i] = setAnimated(651 + 12 * i, 577 + 2, 0, 0, 1, number, 10);

  const char **clockNumber[10];
  clockNumber[0] = Zero;
  clockNumber[1] = Um;
  clockNumber[2] = Dois;
  clockNumber[3] = Tres;
  clockNumber[4] = Quatro;
  clockNumber[5] = Cinco;
  clockNumber[6] = Seis;
  clockNumber[7] = Sete;
  clockNumber[8] = Oito;
  clockNumber[9] = Nove;

  Animated *clock[4];
  for (int i = 0; i < 4; i++)
    clock[i] = setAnimated(738 + 6 * i + ((i / 2) * 4), 581, 0, 0, 1, clockNumber, 10);

  Sprite *dois_pontos = create_sprite(Dois_Pontos, 751, 581, 0, 0);

  //--------MISSILES------------
  //Creating an Animated Missile
  const char **Missilespr[9];
  Missilespr[0] = Missil1;
  Missilespr[1] = Missil2;
  Missilespr[2] = Missil3;
  Missilespr[3] = Missil4;
  Missilespr[4] = Missil5;
  Missilespr[5] = Missil6;
  Missilespr[6] = Missil7;
  Missilespr[7] = Missil8;
  Missilespr[8] = Missil9;
  bool shoot = false;
  Animated *Missile = setAnimated(0, 0, 0, 0, 30, Missilespr, 9);

  //-------CURSOR---------
  Animated *mouse;
  const char **mouseSprites[2];
  mouseSprites[0] = Cursor;
  mouseSprites[1] = Cursor_Left_Click;
  mouse = setAnimated(x_mouse, y_mouse, 0, 0, 0, mouseSprites, 2);

  while ((lsB != ESC_BRKCD) || (msB != 0)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_recieved failed with %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              /* hardware _cou notification */
          if (msg.m_notify.interrupts & irq_timer0) //TIMER0
          {
            if (!loged_in) { //Login page
              memcpy(memBuffer, lg->map, H_RES * V_RES * sizeof(uint8_t) * SYSTEM_BYTES_PER_PIXEL);
              if (!(lsB != ENTER_MKCD) || (msB != 0) || ((leftClick && !leftClickLastFrame) && (x_mouse <= 530 && x_mouse >= 499 && y_mouse >= 395 && y_mouse <= 430))) {
                loged_in = true;
                if (read_time())
                  return 1;
                set_alarm();
              }
              else {
                if ((leftClick && !leftClickLastFrame) && (x_mouse <= 67 && x_mouse >= 33 && y_mouse >= 504 && y_mouse <= 538))
                  lsB = ESC_BRKCD;
                else {
                  for (int i = 0; i < dots_counter && i < 9; i++) {
                    draw_sprite(dt, memBuffer);
                    dt->x = dt->x + 19;
                  }
                  dt->x = 300;
                }
              }
            }

            else { //Loged IN
              frames_counter++;

              //----DRAWS----
              //Background
              memcpy(memBuffer, bg->map, H_RES * V_RES * sizeof(uint8_t) * SYSTEM_BYTES_PER_PIXEL);
              //Icones de abilidades
              draw_sprite(nuke, memBuffer);
              draw_sprite(antivirus, memBuffer);
              //Coins
              SetFrame(numbers[1], (coins % 10));
              SetFrame(numbers[0], coins / 10);
              draw_sprite(numbers[0]->current_sprite, memBuffer);
              draw_sprite(numbers[1]->current_sprite, memBuffer);
              AdvanceFrame(Coin);
              draw_sprite(Coin->current_sprite, memBuffer);
              //Clock
              SetFrame(clock[0], hours / 10);
              SetFrame(clock[1], hours % 10);
              SetFrame(clock[2], minutes / 10);
              SetFrame(clock[3], minutes % 10);
              draw_sprite(clock[0]->current_sprite, memBuffer);
              draw_sprite(clock[1]->current_sprite, memBuffer);
              draw_sprite(clock[2]->current_sprite, memBuffer);
              draw_sprite(clock[3]->current_sprite, memBuffer);
              draw_sprite(dois_pontos, memBuffer);
              //Ability bar
              draw_loading_bar(ability_bars[0], memBuffer, (int) (2 + (64 * coins) / COINS_FOR_NUKE));
              draw_loading_bar(ability_bars[1], memBuffer, (int) (2 + (64 * coins) / COINS_FOR_ANTIVIRUS));
              //Virtual Box
              draw_sprite(vBox, memBuffer);


              
              //SPAWN ENEMY RATE
              if (!(frames_counter % maxInt((int) (ENEMY_SPAWN_RATE(cycles) * 1 / (1 - (antivirus_enabled * ANTIVIRUS_SPAWN_REDUCTION / 100.0))), 1))) {
                for (int i = 0; i < ENEMY_LIMIT; i++) {
                  if (enemies[i] == NULL) {
                    switch (rand() % 16) {
                      case 0:
                        enemies[i] = setAnimated((H_RES >> 1) - 16, V_RES, 0, -0.4 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), AnimatedArr, 4);
                        break;
                      case 1:
                        enemies[i] = setAnimated(252, V_RES, 0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), -0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_2, 4);
                        break;
                      case 2:
                        enemies[i] = setAnimated(((H_RES - V_RES) >> 1) - 39, V_RES, 0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), -0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_3, 4);
                        break;
                      case 3:
                        enemies[i] = setAnimated(0, 449, 0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), -0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_4, 4);
                        break;
                      case 4:
                        enemies[i] = setAnimated(0, (V_RES >> 1) - 16, 0.4 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0, 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), AnimatedArr, 4);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 5:
                        enemies[i] = setAnimated(0, 149 - 22, 0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_2, 4);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 6:
                        enemies[i] = setAnimated(48, 0, 0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_3, 4);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 7:
                        enemies[i] = setAnimated(252, 0, 0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_4, 4);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 8:
                        enemies[i] = setAnimated((H_RES >> 1) - 16, 0, 0, 0.4 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), AnimatedArr, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        break;
                      case 9:
                        enemies[i] = setAnimated(511, 0, -0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_2, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        break;
                      case 10:
                        enemies[i] = setAnimated(H_RES + ((V_RES - H_RES) >> 1) + 39, 0, -0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_3, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        break;
                      case 11:
                        enemies[i] = setAnimated(H_RES, 113, -0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_4, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        break;
                      case 12:
                        enemies[i] = setAnimated(H_RES, (V_RES >> 1) - 16, -0.4 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 0, 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), AnimatedArr, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 13:
                        enemies[i] = setAnimated(H_RES, 449, -0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), -0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_2, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 14:
                        enemies[i] = setAnimated(800 - ((H_RES - V_RES) >> 1), V_RES, -0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), -0.2828 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_3, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 15:
                        enemies[i] = setAnimated(H_RES - 252 - 39, V_RES, -0.15307 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), -0.3696 * (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0))), 20 * (1 / (1 - (antivirus_enabled * ((ANTIVIRUS_SLOW) / 100.0)))), Beetle_4, 4);
                        RotateSprite180(enemies[i]->sprites[0]);
                        RotateSprite180(enemies[i]->sprites[1]);
                        RotateSprite180(enemies[i]->sprites[2]);
                        RotateSprite180(enemies[i]->sprites[3]);
                        RotateSprite90(enemies[i]->sprites[0]);
                        RotateSprite90(enemies[i]->sprites[1]);
                        RotateSprite90(enemies[i]->sprites[2]);
                        RotateSprite90(enemies[i]->sprites[3]);
                        break;
                      case 16:
                        return 0;
                      default:
                        break;
                    }
                    break;
                  }
                }
              }
              if (frames_counter / TIME_BETWEEN_DIFFICULTIES) {
                frames_counter = 0;
                cycles++;
              }

              if (antivirus_counter < ANTIVIRUS_DURATION * 60) {
                antivirus_counter++;
              }
              else {
                antivirus_enabled = false;
                antivirus_counter = 0;
              }
              


              if (lsB == 0x2c & msB == 0) { //Ativar a nuke
                if (coins >= COINS_FOR_NUKE) {
                  if (nuke_cd == 0) {
                    nuke_cd = 90;
                    coins -= COINS_FOR_NUKE;
                    for (int i = 0; i < ENEMY_LIMIT; i++) {
                      if (enemies[i] != NULL) {
                        DestroyAnimated(enemies[i]);
                        for (int j = 0; j < enemies[i]->num_fig; j++) {
                          enemies[i]->sprites[j] = NULL;
                        }
                        enemies[i] = NULL;
                        if (coins < 99) {
                          coins++;
                        }
                      }
                    }
                  }
                }
              }
              if (nuke_cd > 0) {
                nuke_cd--;
              }
              else if (lsB == 0x2d && msB == 0 && !antivirus_enabled) {
                if (coins >= COINS_FOR_ANTIVIRUS) {
                  antivirus_enabled = true;
                  coins -= COINS_FOR_ANTIVIRUS;
                }
              }

              //MENU
              if ((leftClick && !leftClickLastFrame) && (x_mouse < 92 && y_mouse > 570)) //Open/close menu
                menu_open = !menu_open;
              if (menu_open) {
                draw_sprite(menubar, memBuffer);
                draw_sprite(startpressed, memBuffer);
                if ((leftClick && !leftClickLastFrame) && (x_mouse <= 274 && x_mouse >= 250 && y_mouse >= 537 && y_mouse <= 561))
                  lsB = ESC_BRKCD;
                else {
                  if ((leftClick && !leftClickLastFrame) && (x_mouse <= 200 && x_mouse >= 176 && y_mouse >= 537 && y_mouse <= 561)) {
                    loged_in = false;
                    dots_counter = 0;
                  }
                }
              }
              //Processing enimies
              for (int i = 0; i < ENEMY_LIMIT; i++) {
                if (enemies[i] != NULL) {
                  if (enemies[i]->current_sprite->x < -(enemies[i]->current_sprite->width) || enemies[i]->current_sprite->x > H_RES || enemies[i]->current_sprite->y < -(enemies[i]->current_sprite->height) || enemies[i]->current_sprite->y > V_RES) {
                    //if (coins < 99)
                      //coins++;
                    DestroyAnimated(enemies[i]);
                    for (int j = 0; j < enemies[i]->num_fig; j++) {
                      enemies[i]->sprites[j] = NULL;
                    }
                    enemies[i] = NULL;
                  }
                  //Checking hitboxes (should be inside processing enemies code)
                  if (shoot && Missile->cur_fig == 8) {
                    for (int i = 0; i < ENEMY_LIMIT; i++) {
                      if (enemies[i] != NULL) {
                        if (Collision(Missile->current_sprite, enemies[i]->current_sprite)) {
                          if (coins < 99)
                            coins++;
                          DestroyAnimated(enemies[i]);
                          for (int j = 0; j < enemies[i]->num_fig; j++) {
                            enemies[i]->sprites[j] = NULL;
                          }
                          enemies[i] = NULL;
                        }
                      }
                    }
                  }
                  for (int i = 0; i < ENEMY_LIMIT; i++) {
                    if (enemies[i] != NULL) {
                      if (Collision(vBox, enemies[i]->current_sprite) && !perder_jogo)
                        perder_jogo = true;
                    }
                  }
                  if (enemies[i] != NULL) {
                    enemies[i]->current_sprite->x += enemies[i]->current_sprite->xspeed;
                    enemies[i]->current_sprite->y += enemies[i]->current_sprite->yspeed;
                    AdvanceFrame(enemies[i]);
                    draw_sprite(enemies[i]->current_sprite, memBuffer);
                  }
                }
              }

              //----Shooting -------
              if (shoot) {
                if (Missile->cur_fig < 5) {
                  if (Missile->int_to_next_frame == 0) {
                    Missile->current_sprite->x -= 2;
                    Missile->current_sprite->y -= 2;
                  }
                }
                else if (Missile->cur_fig == 4 && Missile->int_to_next_frame == 0)
                  Missile->aspeed = Missile_Speed_2;

                else if (Missile->cur_fig == 5 && Missile->int_to_next_frame == 0)
                  Missile->aspeed = Missile_Speed_3;

                else if (Missile->cur_fig == 7 && Missile->int_to_next_frame == 0)
                  Missile->aspeed = Missile_Speed_4;

                else if (Missile->cur_fig == 8 && Missile->int_to_next_frame == 0)
                  shoot = false;

                if (shoot) {
                  AdvanceFrame(Missile);
                  draw_sprite(Missile->current_sprite, memBuffer);
                }
              }
              else {
                if ((leftClick && !leftClickLastFrame) && !(x_mouse < 92 && y_mouse > 570) && !(x_mouse <= 200 && x_mouse >= 176 && y_mouse >= 537 && y_mouse <= 561)) {
                  shoot = true;
                  Missile->cur_fig = 0;
                  Missile->aspeed = Missile_Speed_1;
                  Missile->int_to_next_frame = Missile_Speed_1;
                  Missile->current_sprite->x = (int) x_mouse - 6;
                  Missile->current_sprite->y = (int) y_mouse - 6;
                  SetFrame(Missile, 0);
                  draw_sprite(Missile->current_sprite, memBuffer);
                }
              }

              if (perder_jogo) {
                draw_sprite(endScreen, memBuffer);
                if ((leftClick && !leftClickLastFrame) && (x_mouse <= 470 && x_mouse >= 407 && y_mouse >= 320 && y_mouse <= 343))
                  lsB = ESC_BRKCD;
                if ((leftClick && !leftClickLastFrame) && (x_mouse <= 382 && x_mouse >= 296 && y_mouse >= 320 && y_mouse <= 343)) {
                  coins = 0;
                  frames_counter = 0;
                  perder_jogo = false;
                  for (int i = 0; i < ENEMY_LIMIT; i++) {
                    if (enemies[i] != NULL) {
                      DestroyAnimated(enemies[i]);
                      for (int j = 0; j < enemies[i]->num_fig; j++)
                        enemies[i]->sprites[j] = NULL;
                      enemies[i] = NULL;
                    }
                  }
                }
              }
            }
            //-------CURSOR---------
            //X
            if ((int) true_x_mouse > (H_RES << 1)) {
              true_x_mouse = (H_RES << 1);
            }
            else if (true_x_mouse < 0) {
              true_x_mouse = 0;
            }
            x_mouse = (float) (((int) true_x_mouse + 1) >> 1);
            //Y
            if ((int) true_y_mouse > (V_RES << 1)) {
              true_y_mouse = (V_RES << 1);
            }
            else if (true_y_mouse < 0) {
              true_y_mouse = 0;
            }
            y_mouse = (float) (((int) true_y_mouse + 1) >> 1);
            //CLicks
            leftClickLastFrame = leftClick;
            if (pp.lb != 0 && leftClickLastFrame == false) {
              AdvanceFrame(mouse);
              leftClick = true;
            }
            else if (pp.rb == 0 && pp.lb == 0 && leftClickLastFrame == true) {
              AdvanceFrame(mouse);
              leftClick = false;
            }
            //Sprite
            set_sprite_position(mouse->current_sprite, x_mouse, y_mouse);
            draw_sprite(mouse->current_sprite, memBuffer);

            //Double buffering
            memcpy(video_mem, memBuffer, H_RES * V_RES * SYSTEM_BYTES_PER_PIXEL * sizeof(uint8_t));
          }
          if (msg.m_notify.interrupts & irq_kbd) //KEYBOARD
          {
            kbc_ih();
            if (!success)
              return 1;

            if ((lsB & MB) == MB) //if it is not makecode
              make = false;
            else {
              make = true;
              if (!loged_in)
                dots_counter++;
            }

            if (msB == MSB_2B) {
              size = 2;
              if (read_lsb) {
                read_lsb = false;
                msB = 0;
              }
              else
                read_lsb = true;
            }
            else {
              msB = 0;
              size = 1;
              if (lsB == BACKSPACE_MKCD && make && !loged_in) {
                if (dots_counter > 1)
                  dots_counter -= 2;
                else if (dots_counter == 1) {
                  dots_counter -= 1;
                }
              }
              if (dots_counter == 0 && !loged_in)
                lsB = 0;
            }
          }

          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            if (read_result == 1)
              return 1;

            else if (read_result == 3)
              continue;

            if (byteNumMouse == 3) {
              byteNumMouse = 0;
              set_packet();
            }
            true_x_mouse += (float) (pp.delta_x);
            true_y_mouse -= (float) (pp.delta_y);
          }

          if (msg.m_notify.interrupts & irq_rtc)
            rtc_ih();
          break;

        default:
          break;
      }
    }
  }

  //DESTROY ALL ANIMATEDS
  //Destroy missile
  DestroyAnimated(Missile);
  for (int j = 0; j < Missile->num_fig; j++) {
    Missile->sprites[j] = NULL;
  }
  Missile = NULL;
  //Destroy Mouse
  DestroyAnimated(mouse);
  for (int j = 0; j < mouse->num_fig; j++) {
    mouse->sprites[j] = NULL;
  }
  mouse = NULL;
  //Destroy enemies
  for (int i = 0; i < ENEMY_LIMIT; i++) {
    if (enemies[i] != NULL) {
      DestroyAnimated(enemies[i]);
      for (int j = 0; j < enemies[i]->num_fig; j++)
        enemies[i]->sprites[j] = NULL;
      enemies[i] = NULL;
    }
  }

  //DESTROY ALL SPRITES
  destroy_sprite(lg);
  lg = NULL;
  destroy_sprite(bg);
  bg = NULL;
  destroy_sprite(vBox);
  vBox = NULL;
  destroy_sprite(menubar);
  menubar = NULL;
  destroy_sprite(startpressed);
  startpressed = NULL;
  destroy_sprite(dt);
  dt = NULL;

  unsubscribeAll();

  return 0;
}

int subscribeAll(uint8_t hook_id_kbd, uint8_t hook_id_timer0, uint8_t hook_id_mouse, uint8_t hook_id_rtc) {
  if (send_command_to_mouse(SET_STRAM_MODE))
    return 1;
  if (send_command_to_mouse(ENABLE_DATA_REPORTING))
    return 1;

  //Subscriptions
  if (keyboard_subscribe_int(&hook_id_kbd))
    return 1;
  if (timer_subscribe_int(&hook_id_timer0))
    return 1;
  if (mouse_subscribe_int(&hook_id_mouse))
    return 1;
  if (rtc_subscribe_int(&hook_id_rtc))
    return 1;

  //Setting graphics mode to 14C
  setGraphicsMode(MODE);

  return 0;
}

int unsubscribeAll() {
  //Unsubscribe
  if (keyboard_unsubscribe_int())
    return 1;
  if (timer_unsubscribe_int())
    return 1;
  if (mouse_unsubscribe_int())
    return 1;
  if (rtc_unsubscribe_int())
    return 1;

  //Disable data reporting
  if (send_command_to_mouse(DISABLE_DATA_REPORTING))
    return 1;
  if (vg_exit())
    return 1;
  return 0;
}
