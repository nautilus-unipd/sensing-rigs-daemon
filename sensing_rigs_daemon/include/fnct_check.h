#ifndef _FNCT_CHECK_H_
#define _FNCT_CHECK_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/statvfs.h>

#include "constants.h"

/**
 * Checks if the remaining free space is enough to store 2 new
 * images.
 * @return  bool    true if there is not enough free space left
 *                  false otherwise
 */
bool check_space(void);

/**
 * Checks if the CPU temperature exceeds a maximum threshold.
 * @return  bool    true if the temperature exceeds the threshold,
 *                  false otherwise.
 */
extern bool check_temperature(void);

/**
 * Checks if the Raspberry PI 5 board has experienced some kind of
 * undervoltage.
 * @return  bool    true if an undervoltage situation has occurred,
 *                  false otherwise.
 */
extern bool check_voltage(void);

#endif //_FNCT_CHECK_H_
