#include "timer.h"
#include "Macros.h"

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
	if (freq < 19  ||  freq > TIMER_FREQ){ 
	  printf("Invalid frequency");
		return 1;
	}
	
  uint8_t var = 0;
  timer_get_conf(timer, &var);
 
  uint8_t counterInitMode = (var >> 4) & 3;

  if (timer > 2 || timer < 0) {
	printf("Invalid timer");
    return 1; 
  }

  if (counterInitMode == 0) 
    return 1;

  //freq = valor que colocamos sys_outb / 1193181
  uint16_t time_value = TIMER_FREQ / freq;

  var = var | TIMER_LSB_MSB;
  uint8_t lsb = 0;
  uint8_t msb = 0;

  util_get_LSB(time_value, &lsb);
  util_get_MSB(time_value, &msb);

  sys_outb(TIMER_CTRL, var);
  sys_outb((timer + TIMER_0), lsb);
  sys_outb((timer + TIMER_0), msb);

  return 0;
}

static int hook_id;
int(timer_subscribe_int)(uint8_t *bit_no)
{
    hook_id = (int)*bit_no;

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id))
        return 1;

    //hook_id;
    return 0;
}

int(timer_unsubscribe_int)()
{
    if (sys_irqrmpolicy(&hook_id))
        return 1;
    return 0;
}
