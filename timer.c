#include "timer.h"
#include "Macros.h"

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t var = 0;
  timer_get_conf(timer, &var);

  if (timer > 2 || timer < 0) 
    return 1;

  //freq = valor que colocamos sys_outb / 1193181
  uint16_t time_value = TIMER_FREQ / freq;
  
  var = var | TIMER_LSB_MSB;
  
  uint8_t LSB = 0;
  uint8_t MSB = 0;
  
  util_get_LSB(time_value, &LSB);
  util_get_MSB(time_value, &MSB);
  
  sys_outb(TIMER_CTRL, var);
  sys_outb((timer + TIMER_0), MSB);
  sys_outb((timer + TIMER_0), LSB);
 
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
