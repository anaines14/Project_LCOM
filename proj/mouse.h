#pragma once     
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

int(readMouseByte)();
void(mouse_ih)();
int(send_command_to_mouse)(uint8_t command);
int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
void(set_packet)();
