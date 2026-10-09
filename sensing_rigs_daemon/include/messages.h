#ifndef _MESSAGES_H_
#define _MESSAGES_H_

/**
 * Custom messages definition.
 */

//--- ERRORs ---
#define ERR_MAIN "[-] Error in 'main()'"
#define ERR_DAEMON_CREATE "[-] Error in 'daemon_create()'"
#define ERR_BLOCK_SIGNALS "Cannot block signals"
#define ERR_UNBLOCK_SIGNALS "Cannot unblock signals"
#define ERR_RCVD_SIGTERM "Received SIGTERM, terminating daemon"
#define ERR_RCVD_SIGABRT "Received SIGABRT, aborting daemon"
#define ERR_INIT_SH "Cannot initialize signal handlers"
#define ERR_UNDERVOLTAGE "Undervoltage detected"
#define ERR_OVERHEATING "Overheating detected"
#define ERR_SPACE_FULL "Not enough free space left"
#define ERR_CREATE_THREAD "Cannot create new thread"
#define ERR_CAMERAS "Cameras not working"
#define ERR_TIMER_CREATE "Unable to create timer"
#define ERR_TIMER_SET "Unable to set timer"

//--- MESSAGEs ---
#define MSG_DAEMON_KILLED "Daemon killed successfully\n\n"
#define MSG_DAEMON_STARTED "Daemon started successfully"
#define MSG_RCVD_SIGHUP1 "Received SIGHUP, disabling image acquisition"
#define MSG_RCVD_SIGHUP2 "Received SIGHUP, enabling image acquisition"
#define MSG_RCVD_SIGUSR1 "Received SIGUSR1, shooting"
#define MSG_RCVD_SIGUSR2 "Received SIGUSR2, checking health"
#define MSG_ALL_GOOD "Good loop"

#endif //_MESSAGES_H_
