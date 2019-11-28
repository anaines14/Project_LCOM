#pragma once
#include <lcom/lcf.h>
#include <lcom/liblm.h>

int subscribe_all(uint8_t hook_id_kbd, uint8_t hook_id_mouse, uint8_t hook_id_timer0);
int unsubscribe_all();
int (game_start)();
int (interrupts)();
