#pragma once
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

/**
 * @brief Receives data from out buffer. Checks if it is valid.
 * 
 * @param data variable to keep the data read 
 * @return Return 0 upon success and non-zero otherwise .
 */
int (read_scancode_kbd)(uint8_t *data);
/**
 * @brief Used to handle interrupts of the keyboard.
 * 
 * @return Void.
 */
void(kbc_ih)();
/**
 * @brief Subscribes and enables keyboard interruptions.
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int(keyboard_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes Keyboard interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise .
 */
int(keyboard_unsubscribe_int)();
