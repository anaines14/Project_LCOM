#pragma once
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

int (read_scancode_kbd)(uint8_t *data); //Tive de alterar o & para * pk  sen�o dava um erro de expected ')' l�
void(kbc_ih)();
int(keyboard_subscribe_int)(uint8_t *bit_no);
int(keyboard_unsubscribe_int)();
