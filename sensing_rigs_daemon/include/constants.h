#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <stdint.h>

// Daemon's shared constants.

/**
 * Constant defining the appending mode for 'fopen()'
 */
extern const char* restrict FM_A;

/**
 * Constant defining the reading mode for 'fopen()'
 */
extern const char* restrict FM_R;

/**
 * Constant defining the writing mode for 'fopen()'
 */
extern const char* restrict FM_W;

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
 * Relative path for the captures, with respect to "DAEMON_PATH"
 */
extern const char* restrict DAEMON_PATH_CAP;

/**
 * Relative path for the captures of the right camera, with respect to "DAEMON_PATH" + "DAEMON_PATH_CAP"
 */
extern const char* restrict DAEMON_PATH_RX;

/**
 * Relative path for the captures of the left camera, with respect to "DAEMON_PATH" + "DAEMON_PATH_CAP"
 */
extern const char* restrict DAEMON_PATH_LX;

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
 * String containing the format for executing the command
 * responsible to take a photo.
 */
extern const char* restrict PHOTO_SHOOT_COMMAND;

/**
 * Maximum temperature threshold for the CPU, measured in mC°.
 */
extern const uint32_t MAX_TEMP;

/**
 * Minimum free disk space required to save 2 images.
 */
extern const uint32_t MIN_FREE_SPACE;

/**
 * Define maximum number of file descriptors to close in case
 * the daemon does not know how many are actually open.
 */
extern const uint16_t MAX_FDS;

/**
 * Maximum size of the temporarily buffer.
 */
#define MAX_ENTRY_SIZE  65

/**
 * Maximum size of a log message.
 */
extern const uint8_t MAX_MSG_SIZE;

/**
 * Maximum buffer size to hold the results of the voltage checks.
 */
extern const uint8_t MAX_VOLT_BUFF_SIZE;

/**
 * Define the maximum number of log entries to buffer before writing to the
 * log file.
 */
extern const uint8_t MAX_LOG_ENTRIES;

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

/**
 * Maximum size of the string holding the command to take a photo.
 */
extern const uint8_t MAX_COMMAND_SIZE;

#endif //_CONSTANTS_H_
