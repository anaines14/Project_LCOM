#include <lcom/lcf.h>
#include "sprites.h"

Sprite *(create_sprite)(const char *pic[], float x, float y, float xspeed, float yspeed) {
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
	sp = NULL;

}

void (set_sprite_position)(Sprite *sp, float xpos, float ypos) {
	sp->x = xpos;
	sp->y = ypos;
}

void (set_sprite)(Sprite *sp, const char* pic[]) {
	xpm_image_t img;
	sp->map = xpm_load(pic, XPM_8_8_8, &img);
}

int (draw_sprite)(Sprite *sp, char *base){
	if (sp == NULL) {
		return 1;
	}
	
	int roundedx = (int)(sp->x + 0.5);
	int roundedy = (int)(sp->y + 0.5);
	uint8_t color[3];
	char *pointer = base;

	int endy = (sp->height < ((V_RES - roundedy)))? sp->height : (V_RES - roundedy);
	int endx = (sp->width < ((H_RES - roundedx)))? sp->width : (H_RES - roundedx);
	
	int starty = maxInt(0, -roundedy);
	int startx = maxInt(0, -roundedx);

	int a = maxInt(roundedx, 0);

	for (int i = starty; i < endy; i++) {
		pointer = base + ((((i + roundedy) * H_RES) + a) * SYSTEM_BYTES_PER_PIXEL); //A posi��o de y * length do ecr� + a posi��o x, tudo * bits_por_pixel do ecra
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

int(draw_loading_bar)(Sprite *sp, char *base, int xPainted) {
  if (sp == NULL) {
    return 1;
  }

  int roundedx = (int) (sp->x + 0.5);
  int roundedy = (int) (sp->y + 0.5);
  uint8_t color[3];
  char *pointer = base;

  int endy = (sp->height < ((V_RES - roundedy))) ? sp->height : (V_RES - roundedy);
  int endx = (sp->width < ((H_RES - roundedx))) ? sp->width : (H_RES - roundedx);

  int starty = maxInt(0, -roundedy);
  int startx = maxInt(0, -roundedx);

  int a = maxInt(roundedx, 0);

  for (int i = starty; i < endy; i++) {
    pointer = base + ((((i + roundedy) * H_RES) + a) * SYSTEM_BYTES_PER_PIXEL); //A posi��o de y * length do ecr� + a posi��o x, tudo * bits_por_pixel do ecra

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
      else if (j < xPainted) {
      *pointer = 0x00;
      pointer++;
      *pointer = 0xd0;
      pointer++;
      *pointer = 0x00;
      pointer++;
    }
      else pointer += SYSTEM_BYTES_PER_PIXEL;
    }
  }
  return 0;
}

void RotateSprite90(Sprite *spr) {
	unsigned newheight = spr->width;
	unsigned newwidth = spr->height;

	uint8_t * newmap = malloc(SYSTEM_BYTES_PER_PIXEL * newheight * newwidth * sizeof(uint8_t));

	unsigned n = spr->width; //Numero de linhas
	unsigned m = spr->height; //Numero de colunas

	for (unsigned i = 0; i < m; i++) {
		for (unsigned j = 0; j < n; j++) {
			*(newmap + SYSTEM_BYTES_PER_PIXEL * (j * newwidth + (newwidth - 1 - i))) = spr->map[SYSTEM_BYTES_PER_PIXEL * (i * spr->width + j)];
			*(newmap + SYSTEM_BYTES_PER_PIXEL * (j * newwidth + (newwidth - 1 - i)) + 1) = spr->map[SYSTEM_BYTES_PER_PIXEL * (i * spr->width + j) + 1];
			*(newmap + SYSTEM_BYTES_PER_PIXEL * (j * newwidth + (newwidth - 1 - i)) + 2) = spr->map[SYSTEM_BYTES_PER_PIXEL * (i * spr->width + j) + 2];
		}
	}

	memcpy(spr->map, newmap, SYSTEM_BYTES_PER_PIXEL * newheight * newwidth * sizeof(uint8_t));
	spr->height = newheight;
	spr->width = newwidth;
}

void RotateSprite180(Sprite *spr) {
	int height = spr->height;
	int width = spr->width;

	uint8_t * newmap = malloc(SYSTEM_BYTES_PER_PIXEL * height * width * sizeof(uint8_t));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			newmap[SYSTEM_BYTES_PER_PIXEL * ((height - i - 1) * width + width - j - 1)] = spr->map[SYSTEM_BYTES_PER_PIXEL * (width * i + j)];
			newmap[SYSTEM_BYTES_PER_PIXEL * ((height - i - 1) * width + width - j - 1) + 1] = spr->map[SYSTEM_BYTES_PER_PIXEL * (width * i + j) + 1];
			newmap[SYSTEM_BYTES_PER_PIXEL * ((height - i - 1) * width + width - j - 1) + 2] = spr->map[SYSTEM_BYTES_PER_PIXEL * (width * i + j) + 2];
		}
	}
	memcpy(spr->map, newmap, SYSTEM_BYTES_PER_PIXEL * height * width * sizeof(uint8_t));
}

bool Collision(Sprite *spr1, Sprite *spr2) {
	if (spr1->x < spr2->x + spr2->width) {
		if (spr1->x + spr1->width > spr2->x) {
			if (spr1->y < spr2->y + spr2->height) {
				if (spr1->y + spr1->height > spr2->y) {
					return true;
				}
			}
		}
	}
	return false;
}
