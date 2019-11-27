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

//GLOBAL KEYBOARD VARIABLES
extern uint8_t status_byte, cmd, data, msB, lsB;
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
const int screenx = 1152;
const int screeny = 864;

int (game_start)() {
	return interrupts();
}


int (interrupts)() {
	int x_mouse = screenx / 2;
	int y_mouse = screeny / 2;

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
	setGraphicsMode(0x11B);

	Sprite fly;
	fly = *create_sprite(flyUp1, 32, 32, 0, 0);
	draw_sprite(&fly, video_mem);

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
					//Se rato sair fora do ecra
					if ((pp.delta_x >> 2) + x_mouse > screenx)
						x_mouse = screenx;
					else if ((pp.delta_x >> 2) + x_mouse <= 0)
						x_mouse = 0;
					else
						x_mouse += pp.delta_x >> 2;

					if ((pp.delta_y >> 2) + y_mouse > screeny)
						y_mouse = screeny;
					else if ((pp.delta_y >> 2) + y_mouse <= 0)
						y_mouse = 0;
					else
						y_mouse += pp.delta_y >> 2;
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
