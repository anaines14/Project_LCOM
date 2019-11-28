#include <lcom/lcf.h>

#include "sprites.h"


Enemy *setEnemy(Sprite *sp, int dir, int tp) {
	Enemy *en = (Enemy *)malloc(sizeof(Enemy));
	Enemy->spr = *sp;
	//Enemy->aspeed = ??
	//Enemy->cur_aspeed = ??
	//Enemy->num_fig = ??
	Enemy->cur_fig = 0
	//Enemy->map = ??
	Enemy->type = tp;
	return 
}
