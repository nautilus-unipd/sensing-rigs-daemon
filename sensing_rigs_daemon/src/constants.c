#include "constants.h"

const char* restrict REDIRECT_FILE = "/dev/null";
const char* restrict DAEMON_BASE_PATH = "/demon/run/";
const char* restrict DAEMON_FILE_LOG = "events.log";
const char* restrict DAEMON_FILE_PID = "sensing_rigs_daemon.pid";
const char* restrict BASE_LOG_ENTRY = "%02d:%02d:%02d, %c, %s\n";

const uint16_t MAX_FDS = 1024;
const uint8_t MAX_BUFF_DIM = 250;
const uint8_t MAX_MSG_SIZE = 50;
