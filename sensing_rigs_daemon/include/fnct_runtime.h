#ifndef _FNCT_RUNTIME_H_
#define _FNCT_RUNTIME_H_

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
extern bool check_space(void);

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

/**
 * Function that capture a photo from one camera, specified as parameter,
 * by opening a new pipe. In order to take a synchronized stereo image,
 * this function must be called in a different thread.
 * @param	void*	Number of camera to use (0 or 1), must be casted
 * 			into a "uint8_t" data type.
 * @return	void*	NULL, but if any error occur the global flag "flag_err_cams"
 * 			is set to false.
 */
extern void* shoot(void*);

#endif //_FNCT_RUNTIME_H_
