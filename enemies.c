#include <lcom/lcf.h>
#include "enemies.h"
#include "sprites.h"
#include "images.h"


Enemy *setEnemy() {
	Enemy *en = (Enemy *)malloc(sizeof(Enemy));
	en->spr[0] = create_sprite(flyUp1, 32, 32, 0, 1);
	en->num_fig = 1;
	en->cur_fig = 0;
	return en;
}

int draw_Enemy(Enemy* en, char *base){
	draw_sprite(en->spr[en->cur_fig], base);
	return 0;
}

int move_Enemy(Enemy* en, char *base){
	if(en->cur_fig == en->num_fig - 1) en->cur_fig = 0;
	en->spr[en->cur_fig + 1]->x += en->spr[en->cur_fig + 1]->xspeed;
	en->spr[en->cur_fig + 1]->y += en->spr[en->cur_fig + 1]->yspeed;
	if (en->spr[en->cur_fig + 1]->x > 0 || en->spr[en->cur_fig + 1]->y > 0 || en->spr[en->cur_fig + 1]->x > H_RES || en->spr[en->cur_fig + 1]->y > V_RES) 
		destroy_enemy(en);
	draw_sprite(en->spr[en->cur_fig], base);
	return 0;
}

void destroy_enemy(Enemy *en){
	if (en == NULL)
		return;
	if (en->spr)
		free(en->spr);
	free(en);
	en = NULL; 
}
