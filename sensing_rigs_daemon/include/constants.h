#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <stdint.h>

// Daemon's shared constants.

/**
 * Define the base format for a log message.
 */
extern const char* restrict BASE_LOG_ENTRY;

/**
 * Default file to redirect STDOUT and STDERR.
 */
extern const char* restrict REDIRECT_FILE;

/**
 * Base path for the daemon runtimes.
 */
extern const char* restrict DAEMON_BASE_PATH;

/**
 * Name of the log file used.
 */
extern const char* restrict DAEMON_FILE_LOG;

/**
 * Name of the pid file used.
 */
extern const char* restrict DAEMON_FILE_PID;

/**
 * Define maximum number of file descriptors to close in case
 * the daemon does not know how many are actually open.
 */
extern const uint16_t MAX_FDS;

/**
 * Maximum size of the temporarily buffer.
 */
extern const uint8_t MAX_BUFF_DIM;

/**
 * Maximum size of a log message.
 */
extern const uint8_t MAX_MSG_SIZE;

#endif //_CONSTANTS_H_
