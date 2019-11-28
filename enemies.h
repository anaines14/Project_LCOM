#pragma once

#include "sprites.h"

typedef struct {
	Sprite spr; //Sprites
	//int direction;  //Direção onde o sprite está a ir em graus
	int type; //Type of enemy
}Enemy;

Enemy *setEnemy(Sprite *sp, int dir, int tp);
