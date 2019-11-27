#pragma once
#include <lcom/lcf.h>
#include "Macros.h"

int(setGraphicsMode)(uint16_t mode);
int define_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color);
void(vg_fill32)(uint32_t color, uint32_t *ptr);
int(draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color);
