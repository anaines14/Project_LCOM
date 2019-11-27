#pragma once
#include <lcom/lcf.h>

int(timer_set_frequency)(uint8_t timer, uint32_t freq);
int(timer_subscribe_int)(uint8_t *bit_no);
int(timer_unsubscribe_int)();
