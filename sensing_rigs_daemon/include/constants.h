#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

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
#define FM_A "a"

/**
 * Constant defining the reading mode for 'fopen()'
 */
#define FM_R "r"

/**
 * Constant defining the writing mode for 'fopen()'
 */
#define FM_W "w"

/**
 * Path containing the root path of the filesystem.
 */
#define ROOT_PATH "/"

/**
 * Default file to redirect STDOUT and STDERR.
 */
#define REDIRECT_PATH "/dev/null"

/**
 * Define the base absolute path for the daemon runtimes.
 */
#define DAEMON_PATH "/demon/run/"

/**
 * Relative path for the log directory, with respect to "DAEMON_PATH".
 */
#define DAEMON_PATH_LOG "sensing_rigs_daemon.log.d/"

/**
 * Relative path for the PID file, with respect to "DAEMON_PATH".
 */
#define DAEMON_PATH_PID "sensing_rigs_daemon.pid"

/**
 * Relative path for the captures, with respect to "DAEMON_PATH"
 */
#define DAEMON_PATH_CAP "captures/"

/**
 * Relative path for the captures of the right camera, with respect to "DAEMON_PATH" + "DAEMON_PATH_CAP"
 */
#define DAEMON_PATH_RX "rx/"

/**
 * Relative path for the captures of the left camera, with respect to "DAEMON_PATH" + "DAEMON_PATH_CAP"
 */
#define DAEMON_PATH_LX "lx/"

/**
 * Base format for a log file, takes the form:
 * YYYY-MM-DD.log
 */
#define BASE_LOG_FILE "%04d-%02d-%02d.log"

/**
 * String containing the absolute path for the file holding
 * the current temperature of the CPU.
 */
#define FILE_PATH_TEMP "/sys/devices/virtual/thermal/thermal_zone0/temp"

/**
 * String containing the command used to get the current voltage
 * state of the Raspberry PI 5 board.
 */
#define VOLT_COMMAND "vcgencmd get_throttled | sed -e 's/^throttled=0x//'"

/**
 * Base format for a log entry, takes the form:
 * hh:mm:ss, <level>, <msg>
 */
#define BASE_LOG_ENTRY "%02d:%02d:%02d, %c, %s\n"

/**
 * String containing the format for executing the command
 * responsible to take a photo.
 */
#define PHOTO_SHOOT_COMMAND "rpicam-still -v 0 --zsl --width 1920 --height 1080 --camera %d --immediate --encoding jpg -o %s"

/**
 * Maximum size of a log message.
 */
#define MAX_MSG_SIZE 50

/**
 * Maximum buffer size to hold the results of the voltage checks.
 */
#define MAX_VOLT_BUFF_SIZE 19

/**
 * First character of a non undervoltage condition for the Raspberry PI 5,
 * must come before the second one.
 */
#define VOLT_CORRECT_1 0x30

/**
 * Second character of a non undervoltage condition for the Raspberry PI 5,
 * must follow the first one.
 */
#define VOLT_CORRECT_2 0x0a

/**
 * Maximum size of the string holding the command to take a photo.
 */
#define MAX_COMMAND_SIZE 138

/**
 * Maximum size for a directory entry.
 */
#define MAX_DIR_SIZE 10

/**
 * Separators for the directory syntax.
 */
#define DIR_SEP_1 '_'
#define DIR_SEP_2 '.'

/**
 * Maximum size for a full directory entry.
 */
#define DIR_PATH_SIZE 512

/**
 * Full string for the directory path.
 */
#define DIR_ENTRY "%s%s%s"

/**
 * Full string for a capture image.
 */
#define FULL_OUT_PATH "%s%s%s%06lu_%ld.jpg"

/**
 * Full string for a log file.
 */
#define FULL_LOG_PATH "%s%slog_%04d_%02d_%02d.txt"

/**
 *
 */
#define FULL_LOG_ENTRY "%02d:%02d:%02d, %c, %s\n"

/**
 * Offset to add for the command.
 */
#define CMD_OFFSET 16

/**
 * Offset to add in order to obtain a valid year.
 */
#define YEAR_OFFSET 1900

#endif //_CONSTANTS_H_
