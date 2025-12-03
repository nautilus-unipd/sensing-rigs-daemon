#ifndef _FNCT_HELPER_H_
#define _FNCT_HELPER_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

#include "constants.h"

/**
 * Global variable used to signal if any error occur in any
 * camera, used in the main loop to get their state.
 */
extern volatile bool flag_err_cams;

/**
 * Function that closes all the file descriptors, to avoid leaving
 * anyone of them open.
 */
extern void close_all_fds();

/**
 * Helper function that returns an instance of a time struct.
 * @return  tm* Struct holding the current datetime informations,
 *              NULL if any error occurs.
 */
extern struct tm* get_local_time(void);

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

#endif //_FNCT_HELPER_H_
