#include "messages.h"

const char* restrict ERR_MAIN = "[-] Error in 'main()'";
const char* restrict ERR_DAEMON_CREATE = "[-] Error in 'daemon_create()'";
const char* restrict ERR_BLOCK_SIGNALS = "Cannot block signals";
const char* restrict ERR_UNBLOCK_SIGNALS = "Cannot unblock signals";
const char* restrict ERR_RCVD_SIGTERM = "Received SIGTERM, terminating daemon";
const char* restrict ERR_RCVD_SIGABRT = "Received SIGABRT, aborting daemon";
const char* restrict ERR_INIT_SH = "Cannot initialize signal handlers";
const char* restrict MSG_DAEMON_STARTED = "Daemon started successfully";
const char* restrict MSG_DAEMON_KILLED = "Daemon killed successfully\n\n";
const char* restrict MSG_RCVD_SIGHUP1 = "Received SIGHUP, disabling image acquisition";
const char* restrict MSG_RCVD_SIGHUP2 = "Received SIGHUP, enabling image acquisition";
