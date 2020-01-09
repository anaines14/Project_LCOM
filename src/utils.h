#pragma once
#include <lcom/lcf.h>

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable. 
 * 
 * @param port the input port that is to be read 
 * @param value address of 8-bit variable to be update with the value read 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (util_sys_inb)(int port, uint8_t *value);
/**
 * @brief Returns the max integer between a and b.
 * 
 * @param a integer 1
 * @param b integer 2
 * @return int 
 */
int maxInt(int a, int b);
/**
 * @brief Converts BCD do decimal
 * 
 * @param BCD argument to be converted
 * @return uint32_t 
 */
uint32_t BCDToDecimal(uint32_t BCD);
