#pragma once
#include "sprites.h"

typedef struct {
	Sprite **spr;		
	int num_fig; 	///< number of pixmaps 
	int cur_fig; 	///< current pixmap 
	//int type;		//Type of enemy
} Enemy;

/*
typedef struct {
	Sprite spr; //Sprites
	//int direction;  //Dire��o onde o sprite est� a ir em graus
	int type; //Type of enemy
}Enemy;*/

Enemy *setEnemy();
int draw_Enemy(Enemy* en, char *base);
int move_Enemy(Enemy* en, char *base);
void destroy_enemy(Enemy *en);
