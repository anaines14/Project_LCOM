#include "timer.h"
#include "Macros.h"


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
