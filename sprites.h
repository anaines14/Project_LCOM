#pragma once
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

typedef struct {
	int x, y; // current position
	int width, height; // dimensions
	int xspeed, yspeed; // current speed
	uint8_t *map; // the pixmap

} Sprite;

Sprite*(create_sprite)(const char *pic[], int x, int y, int xspeed, int yspeed);
int (draw_sprite)(Sprite *sp, char *base);
void (destroy_sprite)(Sprite *sp);

void (set_sprite_position)(Sprite *sp, int xpos, int ypos);

