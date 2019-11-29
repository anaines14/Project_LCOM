#include <lcom/lcf.h>
#include <lcom/liblm.h>

#include "mainLoop.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "VBE.h"
#include "utils.h"
#include "images.h"
#include "sprites.h"
#include "enemies.h"

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

int (game_start)() {
	return interrupts();
}

int (interrupts)() {

	bool menu_open = false;


	int true_x_mouse = H_RES;
	int true_y_mouse = V_RES;
	int x_mouse = (H_RES + 1) >> 1;
	int y_mouse = (V_RES + 1) >> 1;



	int ipc_status, r;
	message msg;

	//Keyboard
	uint8_t hook_id_kbd = KBD_IRQ;
	int irq_kbd = BIT(hook_id_kbd);

	uint8_t size,
		oneByteSc[] = { 0 },     /*Array for scancode of 1byte long*/
		twoBytesSc[] = { 0, 0 }; /*Array for scancode of 2byte long*/
	
	bool make = true, read_lsb = false;

	//Timer
	uint8_t hook_id_timer0 = TIMER0_IRQ;
	int irq_timer0 = BIT(hook_id_timer0);

	//Mouse
	uint8_t hook_id_mouse = MOUSE_IRQ;
	int irq_mouse = BIT(hook_id_mouse);

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

	//Setting graphics mode to 14C	
	setGraphicsMode(MODE);

	
	
	//Creating a memory buffer
	char *memBuffer = (char *)malloc(H_RES * V_RES * SYSTEM_BYTES_PER_PIXEL * sizeof(uint8_t));
	

	//Sprite *background;
	Sprite *bg = create_sprite(background, 0, 0, 0, 0);
	draw_sprite(bg, memBuffer);


	Sprite *flyspr;
	flyspr = create_sprite(flyUp1, 32, 32, 0, 1);
	draw_sprite(flyspr, memBuffer);
	//Enemy *fly = setEnemy(flyspr, 180, 0);

	Sprite *vBox;
	vBox = create_sprite(Virtual_Box, 392, 283, 0, 0);
	draw_sprite(vBox, memBuffer);

	Sprite *menubar;
	menubar = create_sprite(Menu_Bar, 0, 93, 0, 0);

	Sprite *startpressed;
	startpressed = create_sprite(Start_Button_Pressed, 0, 570, 0, 0);
	Sprite *cursor;
	cursor = create_sprite(Cursor, x_mouse, y_mouse, 0, 0);
	draw_sprite(cursor, memBuffer);



	/*
	//Codigo temporario
	xpm_image_t img1;
	uint8_t *cursor = xpm_load(Cursor, XPM_8_8_8, &img1);
	xpm_image_t img2;
	uint8_t *lc_cursor = xpm_load(Cursor_Left_Click, XPM_8_8_8, &img2);
	draw_sprite(cursor, memBuffer);
*/

	while ((lsB != ESC_BRKCD) || (msB != 0))
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_recieved failed with %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{ /* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware _cou notification */
				if (msg.m_notify.interrupts & irq_timer0) //TIMER0
				{
					draw_sprite(bg, memBuffer);
					draw_sprite(flyspr, memBuffer);

					if (true_x_mouse > (H_RES << 1)) {
						true_x_mouse = (H_RES << 1);
					}
					else if (true_x_mouse < 0) {
						true_x_mouse = 0;
					}
					if (true_y_mouse > (V_RES << 1)) {
						true_y_mouse = (V_RES << 1);
					}
					else if (true_y_mouse < 0) {
						true_y_mouse = 0;
					}
					x_mouse = ((true_x_mouse + 1) >> 1);
					y_mouse = ((true_y_mouse + 1) >> 1);
					//set_sprite(cursor, Cursor_Left_Click);
					if (pp.lb != 0) 
						set_sprite(cursor, Cursor_Left_Click);
					else if (pp.rb != 0)
						set_sprite(cursor, Cursor);
					else
						set_sprite(cursor, Cursor);

					if (pp.lb != 0 && (x_mouse < 92 && y_mouse > 570))
						menu_open = true;
					
					if (menu_open) {
						draw_sprite(menubar, memBuffer);
						draw_sprite(startpressed, memBuffer);
					}
					set_sprite_position(cursor, x_mouse, y_mouse);
					draw_sprite(vBox, memBuffer);
					draw_sprite(cursor, memBuffer);
					memcpy(video_mem, memBuffer, H_RES * V_RES * SYSTEM_BYTES_PER_PIXEL * sizeof(uint8_t));
					//Mover os objetos
					flyspr->x += flyspr->xspeed;
					flyspr->y += flyspr->yspeed;
					if (flyspr->x > 0 || flyspr->y > 0 || flyspr->x > H_RES || flyspr->y > V_RES) {
						destroy_sprite(flyspr);
					}
					//Draw sprites, in order of priority
					

				}
				if (msg.m_notify.interrupts & irq_kbd) //KEYBOARD
				{
					kbc_ih();
					if (!success)
						return 1;

					if ((lsB & MB) == MB) //if it is not makecode
						make = false;
					else
						make = true;

					if (msB == MSB_2B)
					{
						size = 2;
						twoBytesSc[0] = msB;
						if (read_lsb)
						{
							twoBytesSc[1] = lsB;
							read_lsb = false;
							msB = 0;
						}
						else
							read_lsb = true;
					}
					else //if scancode is 1 byte long
					{
						msB = 0;
						size = 1;
						oneByteSc[0] = lsB;
					}


				}

				if (msg.m_notify.interrupts & irq_mouse) //Mouse
				{
					mouse_ih();
					if (read_result == 1)
						return 1;

					else if (read_result == 3)
						continue;

					if (byteNumMouse == 3)
					{
						byteNumMouse = 0;
						set_packet();
					}
					true_x_mouse += (pp.delta_x); //+ (pp.delta_x % 4);
					true_y_mouse -= (pp.delta_y); //+ (pp.delta_y % 4);
					////Se rato sair fora do ecra
					//if ((pp.delta_x >> 1) + true_x_mouse > screenx)
					//	true_x_mouse = screenx - 1;
					//else if ((pp.delta_x >> 1) + true_x_mouse <= 0)
					//	true_x_mouse = 0;
					//else
					//	true_x_mouse += pp.delta_x >> 1;

					//if ((pp.delta_y >> 1) + true_y_mouse > screeny)
					//	true_y_mouse = screeny - 1;
					//else if ((pp.delta_y >> 1) + true_y_mouse <= 0)
					//	true_y_mouse = 0;
					//else
					//	true_y_mouse -= pp.delta_y >> 1;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
	}

	//Unsubscribe
	if (keyboard_unsubscribe_int())
		return 1;
	if (timer_unsubscribe_int())
		return 1;
	if (mouse_unsubscribe_int())
		return 1;

	//Disable data reporting
	if (send_command_to_mouse(DISABLE_DATA_REPORTING))
		return 1;
    if (vg_exit())
		return 1;

	return 0;
}
