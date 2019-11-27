#include <lcom/lcf.h>
#include "sprites.h"

//static uint32_t ignoreColor = 0xFF0080;


Sprite *(create_sprite)(const char *pic[], int x, int y,int xspeed, int yspeed) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *)malloc(sizeof(Sprite));
	xpm_image_t img;
	if (sp == NULL)
		return NULL;
	// read the sprite pixmap
	printf(":)");
	return sp;
	sp->map = xpm_load(pic, XPM_8_8_8_8, &img);
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	sp->width = img.width;
	sp->height = img.height;
	sp->map = img.bytes;
	
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;
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

int (draw_sprite)(Sprite *sp, char *base){
	if (sp == NULL) {
		return 1;
	}
	int x = sp->x;
	int y = sp->y;

	uint8_t color[3];
	//uint8_t bytes_per_pixel = 4;

	char *pointer = base;

	int endy = minInt(sp->height, V_RES - y);
	int endx = minInt(sp->width, H_RES - x);
	for (int i = 0; i < endy; i++) {

		pointer = base + ((((i + y) * H_RES) + x) * 4); //A posi��o de y * length do ecr� + a posi��o x, tudo * bits_por_pixel do ecra
		for (int j = 0; j < endx; j++) {

			//Making the color
			for (int k = 0; k < XPM_BYTES_PER_PIXEL; k++) {

				color[k] = sp->map[(i * XPM_BYTES_PER_PIXEL * sp->width) + ((j * XPM_BYTES_PER_PIXEL) + k)];
			}

			//Setting the color unless it is the specified ignore color
			if (!(color[2] == 0xff && color[1] == 0x00 && color[0] == 0x80)) {
			//if(true){
				pointer++; //Passar � frente o transparente
				for (int l = 0; l < 3; l++) {
					*pointer = color[l];
					pointer++;
				}
			}
			else
				pointer += 4;


		}

	}
	/*int x_max = minInt(sp->width + x, H_RES);
	int y_max = minInt(sp->height + y, V_RES);*/
	//int width = minInt(sp->width, H_RES);
	//int height = minInt(sp->height, V_RES);
	//for (int i = y; i < y_max; i++) {
	//	pointer = base + (i * 4 * H_RES);
	//	for (int j = x; j < x_max; j++) {

	//		//Making the color
	//		for (int k = 0; k < XPM_BYTES_PER_PIXEL; k++) {
	//			color[k] = sp->map[((i - y) * XPM_BYTES_PER_PIXEL * sp->width) + (((j - x) * XPM_BYTES_PER_PIXEL) + k)];
	//		}
	//		//Setting the color unless it is the specified ignore color
	//		if (!(color[2] == 0xff && color[1] == 0x00 && color[0] == 0x80)) {
	//			pointer++;
	//			for (int l = XPM_BYTES_PER_PIXEL - 1; l >= 0; l--) {
	//				*pointer = color[l];
	//				pointer++;
	//			}
	//		}
	//		else
	//			pointer += 4;


	//	}
	//	
	//}


	return 0;
}
