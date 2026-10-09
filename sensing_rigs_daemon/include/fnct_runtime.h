#ifndef _FNCT_RUNTIME_H_
#define _FNCT_RUNTIME_H_

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/statvfs.h>

#include "constants.h"
#include "messages.h"
#include "logger.h"

/**
 * Daemon's shared flags.
 *
 * Each bit represents a specific boolean condition shared across
 * different daemon modules.
 *
 * Bit mappings:
 *  bit 0 -> Camera error detected.
 *  bit 1 -> Logging subsystem enabled.
 *  bit 2 -> Main daemon loop running.
 *  bit 3 -> Undervoltage condition detected.
 *  bit 4 -> Overheating condition detected.
 *  bit 5 -> Storage space exhausted.
 *  bit 6 -> Image acquisition enabled.
 *  bit 7 -> SIGALRM signal received.
 *  bit 8 -> SIGUSR2 signal received.
 */

typedef uint16_t daemon_flags_t;
extern _Atomic daemon_flags_t daemon_flags;
//extern _Atomic uint16_t daemon_flags;

/**
 * Camera error flag.
 *
 * Set when one or more cameras fail during image acquisition.
 */
#define FLAG_ERR_CAMS (1U << 0)

/**
 * Logging enabled flag.
 *
 * Set when the logging subsystem is active.
 */
#define FLAG_LOG_ACTIVE (1U << 1)

/**
 * Daemon running flag.
 *
 * Set while the main daemon loop is allowed to execute.
 */
#define FLAG_RUN (1U << 2)

/**
 * Undervoltage flag.
 *
 * Set when the Raspberry Pi reports an undervoltage condition.
 */
#define FLAG_UNDERVOLTAGE (1U << 3)

/**
 * Overheating flag.
 *
 * Set when the CPU temperature exceeds the configured threshold.
 */
#define FLAG_OVERHEATING (1U << 4)

/**
 * Storage full flag.
 *
 * Set when the remaining free disk space falls below the minimum
 * required threshold.
 */
#define FLAG_SPACE_FULL (1U << 5)

/**
 * Image acquisition enabled flag.
 *
 * Set when image acquisition is enabled.
 */
#define FLAG_IMG_ACQ (1U << 6)

/**
 * SIGALRM received flag.
 *
 * Set by the signal handler when a SIGALRM signal is received and
 * cleared once the signal has been processed.
 */
#define FLAG_RCVD_SIGALRM (1U << 7)


/**
 * SIGUSR1 received flag.
 * Set by the signal handler when a SIGUSR1 signal is received and
 * cleared once the signal has been processed.
 */
#define FLAG_RCVD_SIGUSR2 (1U << 8)

/* ====================== FLAG OPERATIONS ====================== */

#define set_flag(f)   (atomic_fetch_or_explicit(&daemon_flags, (f), memory_order_relaxed))
#define clear_flag(f)   (atomic_fetch_and_explicit(&daemon_flags, ~(f), memory_order_relaxed))
#define is_flag(f)    ((atomic_load_explicit(&daemon_flags, memory_order_relaxed) & (f)) != 0)

/**
 * Initialises the sequence counter for each cameras.
 */
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
 * Function that groups all the health checks done on the Raspberry.
 * @return  bool    true if an health check has failed
 *                  false otherwise
 */
extern bool health_checks(void);

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
