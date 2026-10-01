#include "constants.h"

_Atomic daemon_flags_t daemon_flags = 0;

const char* const FM_A = "a";
const char* const FM_R = "r";
const char* const FM_W = "w";
const char* const ROOT_PATH = "/";
const char* const REDIRECT_FILE = "/dev/null";
const char* const DAEMON_PATH = "/home/berry/sensing-rigs-daemon/run/";
const char* const DAEMON_PATH_LOG = "sensing_rigs_daemon.log.d/";
const char* const DAEMON_PATH_PID = "sensing_rigs_daemon.pid";
const char* const DAEMON_PATH_CAP = "captures/";
const char* const DAEMON_PATH_RX = "rx/";
const char* const DAEMON_PATH_LX = "lx/";
const char* const BASE_LOG_FILE = "%04d-%02d-%02d.log";
const char* const BASE_LOG_ENTRY = "%02d:%02d:%02d, %c, %s\n";
const char* const FILE_PATH_TEMP = "/sys/devices/virtual/thermal/thermal_zone0/temp";
const char* const VOLT_COMMAND = "vcgencmd get_throttled | sed -e 's/^throttled=0x//'";
const char* const PHOTO_SHOOT_COMMAND = "rpicam-still -v 0 --zsl --width 1920 --height 1080 --camera %d --immediate --encoding jpg -o %s" ;

const char VOLT_CORRECT_1 = 0x30;
const char VOLT_CORRECT_2 = 0x0a;

const uint8_t MAX_MSG_SIZE = 50;
const uint8_t MAX_VOLT_BUFF_SIZE = 19;
const uint8_t MAX_COMMAND_SIZE = 138;
