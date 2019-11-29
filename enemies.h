#pragma once

#include "sprites.h"

typedef struct {
	Sprite spr;		///< pointer to first Sprite, the one with overall properties
	int aspeed;		///< animation speed 
	int cur_aspeed; ///< current animation speed 
	int num_fig; 	///< number of pixmaps 
	int cur_fig; 	///< current pixmap 
	char **map;     ///< pointer to array of each AnimSprite pixmaps
	int type;		//Type of enemy
} Enemy;

/*
typedef struct {
	Sprite spr; //Sprites
	//int direction;  //Dire��o onde o sprite est� a ir em graus
	int type; //Type of enemy
}Enemy;*/

Enemy *setEnemy(Sprite *sp, int dir, int tp);
