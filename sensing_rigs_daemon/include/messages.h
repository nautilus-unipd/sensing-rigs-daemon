#ifndef _MESSAGES_H_
#define _MESSAGES_H_

/**
 * Custom messages definition.
 */

//--- ERRORs ---
//extern const char* restrict ERR_MAIN;
#define ERR_MAIN "[-] Error in 'main()'"
//extern const char* restrict ERR_DAEMON_CREATE;
#define ERR_DAEMON_CREATE "[-] Error in 'daemon_create()'"
//extern const char* restrict ERR_BLOCK_SIGNALS;
#define ERR_BLOCK_SIGNALS "Cannot block signals"
//extern const char* restrict ERR_UNBLOCK_SIGNALS;
#define ERR_UNBLOCK_SIGNALS "Cannot unblock signals"
//extern const char* restrict ERR_RCVD_SIGTERM;
#define ERR_RCVD_SIGTERM "Received SIGTERM, terminating daemon"
//extern const char* restrict ERR_RCVD_SIGABRT;
#define ERR_RCVD_SIGABRT "Received SIGABRT, aborting daemon"
//extern const char* restrict ERR_INIT_SH;
#define ERR_INIT_SH "Cannot initialize signal handlers"
//extern const char* restrict ERR_UNDERVOLTAGE;
#define ERR_UNDERVOLTAGE "Undervoltage detected"
//extern const char* restrict ERR_OVERHEATING;
#define ERR_OVERHEATING "Overheating detected"
//extern const char* restrict ERR_SPACE_FULL;
#define ERR_SPACE_FULL "Not enough free space left"
//extern const char* restrict ERR_CREATE_THREAD;
#define ERR_CREATE_THREAD "Cannot create new thread"
//extern const char* restrict ERR_CAMERAS;
#define ERR_CAMERAS "Cameras not working"

//--- MESSAGEs ---
//extern const char* restrict MSG_DAEMON_KILLED;
#define MSG_DAEMON_KILLED "Daemon killed successfully\n\n"
//extern const char* restrict MSG_DAEMON_STARTED;
#define MSG_DAEMON_STARTED "Daemon started successfully"
//extern const char* restrict MSG_RCVD_SIGHUP1;
#define MSG_RCVD_SIGHUP1 "Received SIGHUP, disabling image acquisition"
//extern const char* restrict MSG_RCVD_SIGHUP2;
#define MSG_RCVD_SIGHUP2 "Received SIGHUP, enabling image acquisition"
//extern const char* restrict MSG_RCVD_SIGALRM;
#define MSG_RCVD_SIGALRM "Received SIGALRM, shooting"
//extern const char* restrict MSG_ALL_GOOD;
#define MSG_ALL_GOOD "Good loop"

#endif //_MESSAGES_H_
