#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "constants.h"

/**
 * Declare custom type to hold the temporarily buffer
 * and its actual size. 
 */
struct LogBuffer_t;

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
 */
extern void init_logging(void);

/**
 * Disables all logging messages.
 */
extern void disable_logging(void);

/**
 * Enables all logging messages.
 */
extern void enable_logging(void);

/**
 * Appends a new log message to a buffer before writing to
 * the log file. In this way an asynchronous logging mechanism
 * is implemented and the log file is opened only to copy the
 * buffer. Automatically appends to the log message a string
 * containing the timestamp and its log level.
 * @param   LogLevel_t  Gravity level of the log message.
 * @param   const char* Message to write.
 */
extern void append_log(enum LogLevel_t, const char* restrict);

/**
 * Opens the actul log file, which path is a constant, and
 * copies the content of the temporarily buffer to it.
 * After this operation the buffer is freed and the log
 * file descriptor is closed.
 */
extern void write_log(void);

/**
 * Copies one last time the buffer into the log file, then
 * cleans up the variables used in the logging mechanism.
 */
extern void terminate_logging(void);

/**
 * Helper function that sets to 0 the temporarily log buffer
 * and its current size.
 */
extern void _clear_buffer(void);

#endif //_LOGGER_H_
