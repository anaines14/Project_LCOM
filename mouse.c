#include "mouse.h"

struct packet pp;
int read_result;
uint8_t byteNumMouse = 0, bytes[3];

void(set_packet)() {
  uint16_t delta_x = bytes[1];
  if ((bytes[0] & BIT(4)) == BIT(4)) {
    delta_x = delta_x | BIT(8);
  }
  uint16_t delta_y = bytes[2];
  if ((bytes[0] & BIT(5)) == BIT(5)) {
    delta_y = delta_y | BIT(8);
  }
  if ((bytes[0] & BIT(4)) == BIT(4)) {
    delta_x = delta_x | BIT(15) | BIT(14) | BIT(13) | BIT(12) | BIT(11) | BIT(10) | BIT(9);
  }
  if ((bytes[0] & BIT(5)) == BIT(5)) {
    delta_y = delta_y | BIT(15) | BIT(14) | BIT(13) | BIT(12) | BIT(11) | BIT(10) | BIT(9);
  }

  pp.bytes[0] = bytes[0];
  pp.bytes[1] = bytes[1];
  pp.bytes[2] = bytes[2];
  pp.rb = (bytes[0] & BIT(1));
  pp.mb = (bytes[0] & BIT(2));
  pp.lb = (bytes[0] & BIT(0));

  pp.delta_x = delta_x;
  pp.delta_y = delta_y;
  pp.x_ov = (bytes[0] & BIT(6));
  pp.y_ov = (bytes[0] & BIT(7));
}

int(readMouseByte)() {

  uint8_t data, status_byte;

  if (util_sys_inb(STAT_REG, &status_byte)) //if reading status failed
    return 1;

  if (util_sys_inb(OUT_BUF, &data)) //read byte
    return 1;

  if ((status_byte & (PAR_ERR | TO_ERR)) == 0) { //if data valid
    bytes[byteNumMouse] = data;
    byteNumMouse++;
    return 0;
  }
  else //data invalid
    return 1;

  if (byteNumMouse == 1) {
    if (data && BIT(3) == 0) { //Not first byte
      byteNumMouse = 0;
      return 3;
    }
    //else no sync issues
  }

  return 1;
}

void(mouse_ih)() {
  /*Upon interrupt, read byte from the OUT_BUF*/
  read_result = readMouseByte();
}

int(send_command_to_mouse)(uint8_t command)
{
    uint8_t status_byte = 0;
    int calls = 0;

    while (true)
    {
        if (util_sys_inb(STAT_REG, &status_byte)) //if reading status failed
            return 1;

        if ((status_byte & IBF_FULL) == BIT(1))
        { //If input buffer full
            if (calls < 3)
            {
                tickdelay(micros_to_ticks(DELAY_US)); //wait
                calls++;
            }
            else
                return 1;
        }
        else
            break;
    }

    calls = 0;
    bool sent = false;
    uint8_t ack_byte = 0;

    while (!sent)
    {
        sys_outb(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE);

        while (true)
        {
            if (util_sys_inb(STAT_REG, &status_byte)) //if reading status failed
                return 1;

            if ((status_byte & IBF_FULL) == BIT(1)) //If input buffer full
            {
                if (calls < 3)
                {
                    tickdelay(micros_to_ticks(DELAY_US));
                    calls++;
                }
                else
                    return 1;
            }
            else
                break;
        }

        if (sys_outb(OUT_BUF, command))
            return 1;

        do
        {
            if (util_sys_inb(OUT_BUF, &ack_byte))
                return 1;
            
            if (ack_byte == ACK)
                return 0;
            
            else if (ack_byte == NACK)
            {

                if (calls < 3)
                    sent = false;
                else
                    return 1;
            }
            else if (ack_byte == ERROR)
                return 1;
            
            else
                continue;

        } while (true);

        calls++;
    }
    return 0;
}

int hook_id;
int(mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id = (int) *bit_no;
  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  return 0;
}

int(mouse_unsubscribe_int)() {
    if(sys_irqrmpolicy(&hook_id))
        return 1;
    return 0;
  
}
