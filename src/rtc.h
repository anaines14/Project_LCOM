#include "Macros.h"
#include "utils.h"

/**
 * @brief Waits for valid Time. 
 * 
 * @return Void.
 */
void wait_valid_rtc(void);
/**
 * @brief Reads Time from addresses. Puts read time in respective global variables.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int read_time();
/**
 * @brief Subscribes and enables rtc interruptions.
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int(rtc_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes rtc interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(rtc_unsubscribe_int)();
/**
 * @brief Sets an alarm to when seconds are equal to 0.
 * 
 * @return int 
 */
int set_alarm();
/**
 * @brief Subscribes and enables rtc interruptions.
 * 
 * @return Void.
 */
void rtc_ih();
