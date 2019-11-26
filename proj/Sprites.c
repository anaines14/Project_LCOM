#include <lcom/lcf.h>



typedef struct {
	int x, y; // current position
	int width, height; // dimensions
	int xspeed, yspeed; // current speed
	char *map; // the pixmap

	const uint32_t ignoreColor = 0xFF0080;
} Sprite;Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *)malloc(sizeof(Sprite));
	xpm_image_t img;
	if (sp == NULL)
		return NULL;
	// read the sprite pixmap
	sp->map = xpm_load(pic, XPM_INDEXED, &img);
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	sp->width = img.width; sp->height = img.height;

	sp->xspeed = xspeed;
	sp->yspeed = yspeed;
	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	if (sp->map)
		free(sp->map);
	free(sp);
	sp = NULL; // XXX: pointer is passed by value
	// should do this @ the caller
}
static int draw_sprite(Sprite *sp, char *base) {
	
	if (sp = NULL) {
		return 1;
	}
	int x = sp->x;
	int y = sp->y;

	uint8_t[3] color;
	uint8_t bytes_per_pixel = 4;

	char *pointer = base;

	int width = minInt(sp->width, screenx);
	int height = minInt(sp->width, screeny);
	for (int i = y; i < y + height; y++) {
		for (int j = x; j < x + width; x++) {

			//Making the color
			for (int k = 0; k < BMP_BYTES_PER_PIXEL; k++) {
				color[k] = sp[(i - y) * BMP_BYTES_PER_PIXEL * width][((j - x) * BMP_BYTES_PER_PIXEL) + k];
			}
			//Setting the color unless it is the specified ignore color
			if (!(color[3] == 0xff && color[2] == 0x00 && color[3] = 0x80)) {
				for (int l = 0; l < BMP_BYTES_PER_PIXEL; l++) {
					*pointer = color[l];
					pointer++;
				}
			}
			else
				pointer += BMP_BYTES_PER_PIXEL;


		}
		*pointer = base + (i * BMP_BYTES_PER_PIXEL * xscreen);
	}


	return 0;
}
