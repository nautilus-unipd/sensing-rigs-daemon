#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <stdint.h>
#include <stdatomic.h>

/* ================= DAEMON'S SHARED FLAGS ================= */

/**
 * Daemon's shared flags.
 *
 * Each bit represents a specific boolean condition shared across
 * different daemon modules.
 *
 * Bit mapping:
 *   bit 0 -> Camera error detected.
 *   bit 1 -> Logging subsystem enabled.
 *   bit 2 -> Main daemon loop running.
 *   bit 3 -> Undervoltage condition detected.
 *   bit 4 -> Overheating condition detected.
 *   bit 5 -> Storage space exhausted.
 *   bit 6 -> Image acquisition enabled.
 *   bit 7 -> SIGALRM signal received.
 */
typedef uint8_t daemon_flags_t;
extern _Atomic daemon_flags_t daemon_flags;

/**
 * Camera error flag.
 *
 * Set when one or more cameras fail during image acquisition.
 */
#define FLAG_ERR_CAMS      (1U << 0)

/**
 * Logging enabled flag.
 *
 * Set when the logging subsystem is active.
 */
#define FLAG_LOG_ACTIVE    (1U << 1)

/**
 * Daemon running flag.
 *
 * Set while the main daemon loop is allowed to execute.
 */
#define FLAG_RUN           (1U << 2)

/**
 * Undervoltage flag.
 *
 * Set when the Raspberry Pi reports an undervoltage condition.
 */
#define FLAG_UNDERVOLTAGE  (1U << 3)

/**
 * Overheating flag.
 *
 * Set when the CPU temperature exceeds the configured threshold.
 */
#define FLAG_OVERHEATING   (1U << 4)

/**
 * Storage full flag.
 *
 * Set when the remaining free disk space falls below the minimum
 * required threshold.
 */
#define FLAG_SPACE_FULL    (1U << 5)

/**
 * Image acquisition enabled flag.
 *
 * Set when image acquisition is enabled.
 */
#define FLAG_IMG_ACQ       (1U << 6)

/**
 * SIGALRM received flag.
 *
 * Set by the signal handler when a SIGALRM signal is received and
 * cleared once the signal has been processed.
 */
#define FLAG_RCVD_SIGALRM  (1U << 7)

/* ====================== FLAG OPERATIONS ====================== */

#define SET_FLAG(f)   (atomic_fetch_or_explicit(&daemon_flags, (f), memory_order_relaxed))
#define CLEAR_FLAG(f)   (atomic_fetch_and_explicit(&daemon_flags, ~(f), memory_order_relaxed))
#define IS_FLAG(f)    ((atomic_load_explicit(&daemon_flags, memory_order_relaxed) & (f)) != 0)

/* =============== DAEMON'S SHARED CONSTANTS =================== */

/**
 * Maximum temperature threshold for the CPU, measured in mC°.
 */
#define MAX_TEMP 65000

/**
 * Minimum free disk space required to save 2 images.
 */
#define MIN_FREE_SPACE 590000

/**
 * Define maximum number of file descriptors to close in case
 * the daemon does not know how many are actually open.
 */
#define MAX_FDS 1024

/**
 * Maximum size of the temporarily buffer.
 */
#define MAX_ENTRY_SIZE 65

/**
 * Define the maximum number of log entries to buffer before writing to the
 * log file.
 */
#define MAX_LOG_ENTRIES 5

/**
 * Interval for the timer sending the signal to capture a stereo image.
 */
#define INTERVAL_SHOOT 3

/**
 * Constant defining the appending mode for 'fopen()'
 */
extern const char* const FM_A;

/**
 * Constant defining the reading mode for 'fopen()'
 */
extern const char* const FM_R;

/**
 * Constant defining the writing mode for 'fopen()'
 */
extern const char* const FM_W;

/**
 * Path containing the root path of the filesystem.
 */
extern const char* const ROOT_PATH;

/**
 * Default file to redirect STDOUT and STDERR.
 */
extern const char* const REDIRECT_FILE;

/**
 * Define the base absolute path for the daemon runtimes.
 */
extern const char* const DAEMON_PATH;

/**
 * Relative path for the log directory, with respect to "DAEMON_PATH".
 */
extern const char* const DAEMON_PATH_LOG;

/**
 * Relative path for the PID file, with respect to "DAEMON_PATH".
 */
extern const char* const DAEMON_PATH_PID;

/**
 * Relative path for the captures, with respect to "DAEMON_PATH"
 */
extern const char* const DAEMON_PATH_CAP;

/**
 * Relative path for the captures of the right camera, with respect to "DAEMON_PATH" + "DAEMON_PATH_CAP"
 */
extern const char* const DAEMON_PATH_RX;

/**
 * Relative path for the captures of the left camera, with respect to "DAEMON_PATH" + "DAEMON_PATH_CAP"
 */
extern const char* const DAEMON_PATH_LX;

/**
 * Base format for a log file, takes the form:
 * YYYY-MM-DD.log
 */
extern const char* const BASE_LOG_FILE;

/**
 * String containing the absolute path for the file holding
 * the current temperature of the CPU.
 */
extern const char* const FILE_PATH_TEMP;

/**
 * String containing the command used to get the current voltage
 * state of the Raspberry PI 5 board.
 */
extern const char* const VOLT_COMMAND;

/**
 * Base format for a log entry, takes the form:
 * hh:mm:ss, <level>, <msg>
 */
extern const char* const BASE_LOG_ENTRY;

/**
 * String containing the format for executing the command
 * responsible to take a photo.
 */
extern const char* const PHOTO_SHOOT_COMMAND;

/**
 * Maximum size of a log message.
 */
extern const uint8_t MAX_MSG_SIZE;

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

/**
 * Maximum size of the string holding the command to take a photo.
 */
extern const uint16_t MAX_COMMAND_SIZE;

/* =============== END DAEMON'S SHARED CONSTANTS =============== */

#endif //_CONSTANTS_H_
