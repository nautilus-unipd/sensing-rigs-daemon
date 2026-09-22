#ifndef _FNCT_RUNTIME_H_
#define _FNCT_RUNTIME_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/statvfs.h>

#include "constants.h"

void init_seq_counters(void);

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
 * Starts the persistent rpicam-still processes for both cameras in signal mode (-s).
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */
extern int start_camera_processes(void);

/**
 * Terminates the persistent camera processes cleanly.
 */
extern void stop_camera_processes(void);

/**
 * Triggers an instant capture on camera cam (0 or 1) by sending SIGUSR1.
 * @param cam Camera index (0 or 1).
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */
extern int trigger_camera(uint8_t cam);

/**
 * Function that captures a photo from one camera, specified as parameter,
 * by triggering the persistent camera process via SIGUSR1.
 * @param	void*	Number of camera to use (0 or 1), casted to "uint8_t".
 * @return	void*	NULL, sets FLAG_ERR_CAMS on error.
 */
extern void* shoot(void*);

#endif //_FNCT_RUNTIME_H_
