#ifndef _FNCT_DAEMONIZE_H_
#define _FNCT_DAEMONIZE_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "logger.h"
#include "messages.h"
#include "constants.h"
#include "fnct_helper.h"
#include "signal_handler.h"

/**
 * Function that creates and initialise a new daemon process
 * from a parent caller, can be called inside the main function.
 * @return  int EXIT_FAILURE if any error occurs,
 *              EXIT_SUCCESS otherwise.
 */
extern int daemon_create(void);

/**
 * Function that opens the PID file.
 * @param   bool    flag that represents the mode for the new file:
 *                  true for "write",
 *                  false for "read".
 * @return  FILE*   pointer to file stream, NULL in case of errors.
 */
extern FILE* open_pid_file(bool);

/**
 * Function that checks if the PID file already exists.
 * @return  bool    true if the PID file exists and contains a valid value,
 *                  false otherwise.
 */
extern bool check_pid_file(void);

/**
 * Function that creates and writes the PID of the daemon to the PID file.
 * @return  bool    true if the creation is succesful,
 *                  false otherwise.
 */
extern bool create_pid_file(void);

/**
 * Function that closes the PID file. This one must be called when the daemon quits.
 * @return  bool    true if the closing was succesful,
 *                  false otherwise.
 */
extern void close_pid_file(void);

/**
 * Function that cleans up all the components of a daemon.
 * @param	bool	Flag that clears the PID file if set to true.
 */
extern void daemon_terminate(bool);

#endif //_FNCT_DAEMONIZE_H_
