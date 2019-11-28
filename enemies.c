#include <lcom/lcf.h>

#include "sprites.h"
#include "enemies.h"


Enemy *setEnemy(Sprite *sp, int dir, int tp) {
	Enemy *en = (Enemy *)malloc(sizeof(Enemy));
	Enemy->spr = *sp;
	//Enemy->direction = dir;
	Enemy->type = tp;
	return 
}

