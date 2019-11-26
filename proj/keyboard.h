#pragma once
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

int (read_scancode_kbd)(uint8_t *data);
void(kbc_ih)();
int(keyboard_subscribe_int)(uint8_t *bit_no);
int(keyboard_unsubscribe_int)();
