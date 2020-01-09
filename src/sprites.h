#pragma once
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

/**
 * @brief Represents a sprite.
 * 
 * @struct Sprite
 */
typedef struct {
	float x, y;///< Current position  
	int width, height; ///< Dimentions  
	float xspeed, yspeed; ///< Current speed  
	uint8_t *map; ///< The pix map  
} Sprite;

/**
 * @brief Creates a Sprite object.
 * 
 * @param pic array of xpms of sprite
 * @param x horizontal position of the sprite
 * @param y vertical position of the sprite
 * @param xspeed horizontal speed of the sprite
 * @param yspeed vertical speed of the sprite
 * @return Sprite*
 */
Sprite*(create_sprite)(const char *pic[], float x, float y, float xspeed, float yspeed);
/**
 * @brief Draws sprite passed in arguments in base.
 * 
 * @param sp Sprite to be drawn.
 * @param base Where sprite is going to be drawn.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (draw_sprite)(Sprite *sp, char *base);
/**
 * @brief Frees memory occupied by sprite.
 * 
 * @param sp Sprite to free.
 * @return Void.
 */
void (destroy_sprite)(Sprite *sp);
/**
 * @brief Changes sprite's xpm to pic.
 * 
 * @param sp sprite to receive xpm
 * @param pic xpm to be put in sprite
 * @return Void.
 */
void (set_sprite)(Sprite *sp, const char* pic[]);
/**
 * @brief Changes sprite position to cpos and ypos.
 * 
 * @param sp sprite which position is going to be changed
 * @param xpos new horizontal position
 * @param ypos new vertical position
 * @return Void.
 */
void (set_sprite_position)(Sprite *sp, float xpos, float ypos);
/**
 * @brief Rotates sprite 90 degrees.
 * 
 * @param spr Sprite to be rotated.
 * @return Void.
 */
void RotateSprite90(Sprite *spr);
/**
 * @brief Rotates sprite spr 180 degrees.
 * 
 * @param spr Sprite to be rotated.
 * @return Void.
 */
void RotateSprite180(Sprite *spr);
/**
 * @brief Checks if there was a collision between spr1 and spr2.
 * 
 * @param spr1 sprite 1
 * @param spr2 sprite 2
 * @return true if spr1 and spr2 collided
 * @return false if pr2 and spr2 did not collide.
 */
bool Collision(Sprite *spr1, Sprite *spr2);
/**
 * @brief Draws loading bar
 * 
 * @param sp Sprite of loading bar
 * @param base Where the loading bar is going to be drawn
 * @param xGreen Quantity in bar
 * @return Return 0 upon success and non-zero otherwise.
 */
int(draw_loading_bar)(Sprite *sp, char *base, int xGreen);
