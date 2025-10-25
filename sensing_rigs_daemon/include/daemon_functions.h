#ifndef _DAEMON_FUNCTIONS_H_
#define _DAEMON_FUNCTIONS_H_

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
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
 * Function that cleans up all the components of a daemon.
 */
void daemon_terminate(void);

#endif //_DAEMON_FUNCTIONS_H_
