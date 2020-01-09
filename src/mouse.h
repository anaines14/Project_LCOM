#pragma once     
#include <lcom/lcf.h>
#include "Macros.h"
#include "utils.h"

/**
 * @brief Reads mouse data from out buffer. Checks if data is valid.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(readMouseByte)();
/**
 * @brief Subscribes and enables mouse interruptions.
 * 
 * @return Void.
 */
void(mouse_ih)();
/**
 * @brief Sends command passed as argument to mouse.
 * 
 * @param command Command to send to mouse.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(send_command_to_mouse)(uint8_t command);
/**
 * @brief Subscribes and enables mouse interruptions.
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_unsubscribe_int)();
/**
 * @brief Functions responsible for modifying packet struct by parsing packets received by from the mouse.
 * 
 * @return Void.
 */
void(set_packet)();
