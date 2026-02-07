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
 * Function that tries to open a specified file path with the specified mode.
 * @param	const char*	String containing the file path to open.
 * @param	const char*	String containint the mode to open the file path.
 * @return	FILE*	Pointer to a new file stream corresponding to the file path,
 * 			NULL if any error occurs.
 */
extern FILE* open_file(const char*, const char*);

/**
 * Helper function that returns an instance of a time struct.
 * @return  tm* Struct holding the current datetime informations,
 *              NULL if any error occurs.
 */
extern struct tm* get_local_time(void);

#endif //_FNCT_HELPER_H_
