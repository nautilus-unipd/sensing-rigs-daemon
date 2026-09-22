#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "constants.h"
#include "fnct_helper.h"

#define LOG_FILENAME_MAX 256

/**
 * Defines custom enumeration to represent different
 * levels of logging, from least important to most
 * critical.
 */
enum LogLevel_t
{
    DEBUG = 'D',
    INFO = 'I',
    WARNING = 'W',
    ERROR = 'E'
};

/**
 * Initializes the logging system. Does not open the actual 
 * log file, but initializes the buffer that will temporarily
 * hold some log messages.
 * @return  int EXIT_FAILURE if any error occurs,
 *              EXIT_SUCCESS otherwise.
 */
int init_logging(void);

/**
 * Disables all logging messages.
 */
void disable_logging(void);

/**
 * Enables all logging messages.
 */
void enable_logging(void);

/**
 * Appends a new log message to a buffer before writing to
 * the log file. In this way an asynchronous logging mechanism
 * is implemented and the log file is opened only to copy the
 * buffer. Automatically appends to the log message a string
 * containing the timestamp and its log level.
 * @param   LogLevel_t  Gravity level of the log message.
 * @param   const char* Message to write.
 * @return  int EXIT_FAILURE if any error occurs,
 *              EXIT_SUCCESS otherwise.
 */
int append_log(enum LogLevel_t level, const char* restrict msg);

/**
 * Opens the actual log file, which path is a constant, and
 * copies the content of the temporarily buffer to it.
 * After this operation the buffer is freed and the log
 * file descriptor is closed.
 * @return  int EXIT_FAILURE if any error occurs,
 *              EXIT_SUCCESS otherwise.
 */
extern int write_log(void);

/**
 * Copies one last time the buffer into the log file, then
 * cleans up the variables used in the logging mechanism.
 */
extern void terminate_logging(void);

#endif
