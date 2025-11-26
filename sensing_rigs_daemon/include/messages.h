#ifndef _MESSAGES_H_
#define _MESSAGES_H_

/**
 * Custom messages definition.
 */

// --- ERRORs ---
extern const char* restrict ERR_MAIN;
extern const char* restrict ERR_DAEMON_CREATE;
extern const char* restrict ERR_BLOCK_SIGNALS;
extern const char* restrict ERR_UNBLOCK_SIGNALS;
extern const char* ERR_RCVD_SIGTERM;
extern const char* ERR_RCVD_SIGABRT;
extern const char* ERR_INIT_SH;

// --- MESSAGEs ---
extern const char* restrict MSG_DAEMON_KILLED;
extern const char* restrict MSG_DAEMON_STARTED;

#endif //_MESSAGES_H_
