#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>

#include "logger.h"
#include "fnct_daemonize.h"

/**
 * Global variable used to signal if a SIGHUP has been
 * received in the main loop.
 */
extern volatile bool rcvd_sighup;

/**
 * Constant array holding the signals to block.
 */
extern const uint8_t BLCK_SIG[];

/**
 * Initializes the signal handler.
 * @return  int EXIT_FAILURE if an error occurs,
 *              EXIT_SUCCESS otherwise.
 */
extern int init_sig_handler(void);

/**
 * Depending on the signal number it handles accordingly the
 * received signal, performing different acions.
 * @param   int Number of the signal received.
 */
extern void sig_handler(int);

/**
 * Momentarily blocks the reception of the signals inside BLCK_SIG
 * in order to perform some critical action without interruptions.
 * When the critical zone ends the function "unblock_signals" must
 * be called to resume the normal handlings of signals.
 * @return  int EXIT_FAILURE if an error occurs,
 *              EXIT_SUCCESS otherwise.
 */
extern int block_signals(void);

/**
 * Unblocks the signals inside BLCK_SIG from current signal mask.
 * A blocked signal will be handled when this function is called.
 * @return  int EXIT_FAILURE if an error occurs,
 *              EXIT_SUCCESS otherwise.
 */
extern int unblock_signals(void);

#endif //_HEADER_H_
