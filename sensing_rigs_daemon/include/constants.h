#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <stdint.h>

// Daemon's shared constants.

/**
 * Default file to redirect STDOUT and STDERR.
 */
extern const char* restrict REDIRECT_FILE;

/**
 * Define the base absolute path for the daemon runtimes.
 */
extern const char* restrict DAEMON_PATH;

/**
 * Relative path for the log directory, with respect to "DAEMON_PATH".
 */
extern const char* restrict DAEMON_PATH_LOG;

/**
 * Relative path for the PID file, with respect to "DAEMON_PATH".
 */
extern const char* restrict DAEMON_PATH_PID;

/**
 * Base format for a log file, takes the form:
 * YYYY-MM-DD.log
 */
extern const char* restrict BASE_LOG_FILE;

/**
 * Base format for a log entry, takes the form:
 * hh:mm:ss, <level>, <msg>
 */
extern const char* restrict BASE_LOG_ENTRY;

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
