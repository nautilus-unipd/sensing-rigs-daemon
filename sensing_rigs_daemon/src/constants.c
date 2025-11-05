#include "constants.h"

const char* restrict REDIRECT_FILE = "/dev/null";
const char* restrict DAEMON_PATH_LOG = "/demon/run/sensing_rigs_daemon.log.d/%04d-%02d-%02d.log";
const char* restrict DAEMON_PATH_PID = "/demon/run/sensing_rigs_daemon.pid";
const char* restrict BASE_LOG_ENTRY = "%02d:%02d:%02d, %c, %s\n";

const uint16_t MAX_FDS = 1024;
const uint8_t MAX_BUFF_DIM = 250;
const uint8_t MAX_MSG_SIZE = 50;
