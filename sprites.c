#include <lcom/lcf.h>
#include "sprites.h"

Sprite *(create_sprite)(const char *pic[], int x, int y,int xspeed, int yspeed) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *)malloc(sizeof(Sprite));
	xpm_image_t img;
	
	if (sp == NULL)
		return NULL;
	
	// read the sprite pixmap
	sp->map = xpm_load(pic, XPM_8_8_8, &img);
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	
	sp->width = img.width;
	sp->height = img.height;
	sp->map = img.bytes;
	
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;

	sp->x = x;
	sp->y = y;
	
	return sp;
}

void (destroy_sprite)(Sprite *sp) {
	if (sp == NULL)
		return;
	if (sp->map)
		free(sp->map);
	free(sp);
	sp = NULL; // XXX: pointer is passed by value
	// should do this @ the caller
}

void setSpritePosition(Sprite *sp, int xpos, int ypos) {
    sp->x = xpos;
    sp->y = ypos;
}

int (draw_sprite)(Sprite *sp, char *base){
	if (sp == NULL) {
		return 1;
	}

	uint8_t color[3];
	char *pointer = base;

	int endy = (sp->height < ((V_RES - sp->y)))? sp->height : (V_RES - sp->y);
	int endx = (sp->width < ((H_RES - sp->x)))? sp->width : (H_RES - sp->x);
	
	int starty = maxInt(0, -sp->y);
	int startx = maxInt(0, -sp->x);


	for (int i = starty; i < endy; i++) {
		pointer = base + ((((i + sp->y) * H_RES) + sp->x) * 3); //A posi��o de y * length do ecr� + a posi��o x, tudo * bits_por_pixel do ecra
		for (int j = startx; j < endx; j++) {
			//Making the color
			for (int k = 0; k < XPM_BYTES_PER_PIXEL; k++) {
				color[k] = sp->map[(i * XPM_BYTES_PER_PIXEL * sp->width) + ((j * XPM_BYTES_PER_PIXEL) + k)];
			}
			//Setting the color unless it is the specified ignore color
			if (!(color[2] == 0xff && color[1] == 0x00 && color[0] == 0x80)) {
				for (int l = 0; l < SYSTEM_BYTES_PER_PIXEL; l++) {
					*pointer = color[l];
					pointer++;
				}
			}
			else
				pointer += SYSTEM_BYTES_PER_PIXEL;
		}
	}
	return 0;
}
