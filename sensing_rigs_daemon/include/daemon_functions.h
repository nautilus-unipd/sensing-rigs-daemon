#ifndef _DAEMON_FUNCTIONS_H_
#define _DAEMON_FUNCTIONS_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "logger.h"
#include "messages.h"
#include "constants.h"

/**
 * Function that creates and initialise a new daemon process
 * from a parent caller, can be called inside the main function.
 */
void daemon_create(void);

/**
 * Function that closes all the file descriptors, to avoid leaving
 * anyone of them open.
 */
void _close_all_fds(void);

/**
 * Function that checks if the PID file already exists.
 * @return  bool    true if the PID file exists and contains a valid value,
 *                  false otherwise.
 */
bool pid_check_file(void);

/**
 * Function that creates and writes the PID of the daemon to the PID file.
 * @return  bool    true if the creation is succesful,
 *                  false otherwise.
 */
bool pid_create_file(void);

/**
 * Function that closes the PID file. This one must be called when the daemon quits.
 * @return  bool    true if the closing was succesful,
 *                  false otherwise.
 */
void pid_close_file(void);

/**
 * Function that cleans up all the components of a daemon.
 */
void daemon_terminate(void);

#endif //_DAEMON_FUNCTIONS_H_
