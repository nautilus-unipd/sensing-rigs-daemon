#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <stdint.h>

// Daemon's shared constants.

/**
 * Path containing the root path of the filesystem.
 */
extern const char* restrict ROOT_PATH;

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
 * String containing the absolute path for the file holding
 * the current temperature of the CPU.
 */
extern const char* restrict FILE_PATH_TEMP;

/**
 * String containing the command used to get the current voltage
 * state of the Raspberry PI 5 board.
 */
extern const char* restrict VOLT_COMMAND;

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

/**
 * Minimum free disk space required to save 2 images.
 */
extern const unsigned long int MIN_FREE_SPACE;

/**
 * Maximum temperature threshold for the CPU, measured in mC°.
 */
extern const uint32_t MAX_TEMP;

/**
 * Maximum buffer size to hold the results of the voltage checks.
 */
extern const uint8_t MAX_VOLT_BUFF_SIZE;

/**
 * First character of a non undervoltage condition for the Raspberry PI 5,
 * must come before the second one.
 */
extern const char VOLT_CORRECT_1;

/**
 * Second character of a non undervoltage condition for the Raspberry PI 5,
 * must follow the first one.
 */
extern const char VOLT_CORRECT_2;

#endif //_CONSTANTS_H_
