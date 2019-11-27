#include "keyboard.h"
#include <lcom/lcf.h>
uint8_t  msB, lsB;
bool success;

int (read_scancode_kbd)(uint8_t *data){
	uint8_t status_byte;

	if(util_sys_inb(STAT_REG, &status_byte)) //if reading status failed
		return 1;
		
	if((status_byte & MOUSE) == MOUSE)
		return 1;
	
	if (status_byte & OBF){ //if out buffer full
		if(util_sys_inb(OUT_BUF, data)) /*if failed*/
			return 1;
			
		if((status_byte & (PAR_ERR | TO_ERR)) == 0){ //if data valid
			return 0;
		}
		else 
			return 1;
		}
	return 2;	
}

void(kbc_ih)()
{
	success = true;
	int calls = 0;
	uint8_t  *data;
	data = malloc(1);
	while (calls < 3)
	{
		success = !read_scancode_kbd(data);
		if (success == true)
		{
			if (*data == MSB_2B) /*if scancode is 16 bits*/
				msB = *data;
			else
				lsB = *data;
			return;
		}
		calls++;
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return;
}

static int hook_id;
int(keyboard_subscribe_int)(uint8_t *bit_no)
{
	hook_id = (int)*bit_no;
	sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
	return 0;
}

int(keyboard_unsubscribe_int)()
{
	if (sys_irqrmpolicy(&hook_id))
		return 1;
	return 0;
}
