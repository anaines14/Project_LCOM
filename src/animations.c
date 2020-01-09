#include <lcom/lcf.h>

#include "animations.h"



Animated *setAnimated(float x, float y, float xspeed, float yspeed, unsigned int animationspeed, const char**xpms[],  unsigned numxpms) {
	Animated *en = (Animated *)malloc(sizeof(Animated));
	en->num_fig = numxpms;
	
	en->sprites = (Sprite **)malloc(sizeof(Sprite *) * numxpms);
	
	for (unsigned i = 0; i < numxpms; i++) {
		en->sprites[i] = create_sprite(xpms[i], x, y, xspeed, yspeed);
	}
	en->current_sprite = en->sprites[0];
	en->current_sprite->x = x;
	en->current_sprite->y = y;
	en->current_sprite->xspeed = xspeed;
	en->current_sprite->yspeed = yspeed;
	en->cur_fig = 0;
	en->int_to_next_frame = animationspeed;
	en->aspeed = animationspeed;
	return en;
} 


void AdvanceFrame(Animated *en) {
	if (en->int_to_next_frame) {
		en->int_to_next_frame--;
	}
	else {
		en->int_to_next_frame = en->aspeed;
		en->cur_fig++;
		if (en->cur_fig >= en->num_fig)
			en->cur_fig = 0;
		float x = en->current_sprite->x;
		float y = en->current_sprite->y;
		float xspeed = en->current_sprite->xspeed;
		float yspeed = en->current_sprite->yspeed;
		en->current_sprite = en->sprites[en->cur_fig];
		en->current_sprite->x = x;
		en->current_sprite->y = y;
		en->current_sprite->xspeed = xspeed;
		en->current_sprite->yspeed = yspeed;
	}
}

void SetFrame(Animated *en, unsigned int nextfig) {
	en->cur_fig = nextfig;
	float x = en->current_sprite->x;
	float y = en->current_sprite->y;
	float xspeed = en->current_sprite->xspeed;
	float yspeed = en->current_sprite->yspeed;
	en->current_sprite = en->sprites[en->cur_fig];
	en->current_sprite->x = x;
	en->current_sprite->y = y;
	en->current_sprite->xspeed = xspeed;
	en->current_sprite->yspeed = yspeed;
}

void DestroyAnimated(Animated *en) {
	if (en == NULL) {
		return;
	}
	//destroy_sprite(en->current_sprite);
	if (en->sprites) {
		for (int i = 0; i < en->num_fig; i++)
			//free(en->sprites[i]);
			destroy_sprite(en->sprites[i]);

		free(en->sprites);
	}
	free(en);
	en = NULL;
}
