#include "proj.h"

extern bool success;
extern uint8_t msB, lsB, byteNumMouse;
extern int read_result;
extern struct packet pp;

int interrupts()
{
    int ipc_status, r;
    message msg;

    //Keyboard
    uint8_t hook_id_kbd = KBD_IRQ;
    int irq_kbd = BIT(hook_id_kbd);

    uint8_t size,
        oneByteSc[] = {0},     /*Array for scancode of 1byte long*/
        twoBytesSc[] = {0, 0}; /*Array for scancode of 2byte long*/
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

    while (true)
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
            case HARDWARE:                                /* hardware _cou notification */
                if (msg.m_notify.interrupts & irq_timer0) //TIMER0
                {
                    timer_int_handler();
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

    return 0;
}
