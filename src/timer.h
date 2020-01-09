#pragma once
#include <lcom/lcf.h>

/**
 * @brief Subscribes and enables timer0 interruptions.
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int(timer_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes timer0 interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(timer_unsubscribe_int)();
