#include "constants.h"

const char* restrict FM_A = "a"; 
const char* restrict FM_R = "r"; 
const char* restrict FM_W = "w";
const char* restrict ROOT_PATH = "/";
const char* restrict REDIRECT_FILE = "/dev/null";
const char* restrict DAEMON_PATH = "/home/berry/sensing-rigs-daemon/run/";
const char* restrict DAEMON_PATH_LOG = "sensing_rigs_daemon.log.d/";
const char* restrict DAEMON_PATH_PID = "sensing_rigs_daemon.pid";
const char* restrict DAEMON_PATH_CAP = "captures/";
const char* restrict DAEMON_PATH_RX = "rx/";
const char* restrict DAEMON_PATH_LX = "lx/";
const char* restrict BASE_LOG_FILE = "%04d-%02d-%02d.log";
const char* restrict BASE_LOG_ENTRY = "%02d:%02d:%02d, %c, %s\n";
const char* restrict FILE_PATH_TEMP = "/sys/devices/virtual/thermal/thermal_zone0/temp";
const char* restrict VOLT_COMMAND = "vcgencmd get_throttled | sed -e 's/^throttled=0x//'";
const char* restrict PHOTO_SHOOT_COMMAND = "rpicam-still -v 0 --zsl --width 1920 --height 1080 --camera %d --timestamp --immediate -o %s%s%s";

const char VOLT_CORRECT_1 = 0x30;
const char VOLT_CORRECT_2 = 0x0a;

const uint32_t MIN_FREE_SPACE = 590000;
const uint32_t MAX_TEMP = 65000;
const uint16_t MAX_FDS = 1024;
const uint8_t MAX_BUFF_DIM = 250;
const uint8_t MAX_MSG_SIZE = 50;
const uint8_t MAX_VOLT_BUFF_SIZE = 19;
const uint8_t MAX_COMMAND_SIZE = 138;
