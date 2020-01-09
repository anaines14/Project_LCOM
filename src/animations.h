#pragma once
#include "sprites.h"

/**
 * @brief Represents a object which is animated.
 * @struct Animated
 */
typedef struct {		
	int aspeed;		///< animation speed  
	int num_fig; 	///< number of pixmaps 
	int cur_fig; 	///< current Sprite 
	Sprite **sprites;     ///< pointer to all Sprites
	Sprite *current_sprite; //Current_sprite
	int type; //Type of enemy
	int int_to_next_frame; //número de interrupts do timer 0 até ao próximo frame
} Animated;

/**
 * @brief Creates an Animated object with the given parameters.
 * 
 * @param x position horizontaly
 * @param y position verticaly
 * @param xspeed velocity vector in x
 * @param yspeed velocity vector iny
 * @param animationspeed 
 * @param xpms Array of xpms of Animated
 * @param numxpms Number of xpms of Animated
 * @return Animated* 
 */
Animated *setAnimated(float x, float y, float xspeed, float yspeed, unsigned int animationspeed, const char**xpms[], unsigned numxpms);
/**
 * @brief If int_to_next_frame is 0, the function updates the Animated with its next sprite updating all variables necessary.
 * 
 * @param en Animated to be updated
 * @return Void.
 */
void AdvanceFrame(Animated *en);
/**
 * @brief Makes the Animated* en skip to its sprite with number nextfig. 
 * 
 * @param en Aniimated to be updated
 * @param nextfig number of the sprite
 * @return Void.
 */
void SetFrame(Animated *en, unsigned int nextfig);
/**
 * @brief Frees Memory occupied by Animated object.
 * 
 * @param en Animated to free.
 * @return Void.
 */
void DestroyAnimated(Animated *en);
