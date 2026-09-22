#include "logger.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static char log_buffer[MAX_LOG_ENTRIES][MAX_ENTRY_SIZE];
static uint16_t log_entries = 0;

// INTERNAL SAFE HELPERS

static struct tm* get_time_safe(void)
{
    time_t now = time(NULL);
    return localtime(&now);
}

// BUFFER MANAGEMENT

static void _clear_buffer(void)
{
    for (uint16_t i = 0; i < MAX_LOG_ENTRIES; i++)
        log_buffer[i][0] = '\0';

    log_entries = 0;
}

// INIT / CONTROL

int init_logging(void)
{
    pthread_mutex_lock(&mutex);
    SET_FLAG(FLAG_LOG_ACTIVE);
    log_entries = 0;
    pthread_mutex_unlock(&mutex);

    return EXIT_SUCCESS;
}

void disable_logging(void)
{
    pthread_mutex_lock(&mutex);
    CLEAR_FLAG(FLAG_LOG_ACTIVE);
    pthread_mutex_unlock(&mutex);
}

void enable_logging(void)
{
    pthread_mutex_lock(&mutex);
    SET_FLAG(FLAG_LOG_ACTIVE);
    pthread_mutex_unlock(&mutex);
}

// ROTATION (DAILY)

static void build_log_filename(char* out, size_t size)
{
    struct tm* t = get_time_safe();

    snprintf(out, size,
             "%s%slog_%04d_%02d_%02d.txt",
             DAEMON_PATH,
             DAEMON_PATH_LOG,
             t->tm_year + 1900,
             t->tm_mon + 1,
             t->tm_mday);
}

// APPEND LOG

int append_log(enum LogLevel_t level, const char* restrict msg)
{
    if (!IS_FLAG(FLAG_LOG_ACTIVE) || !msg)
        return EXIT_SUCCESS;

    size_t len = strlen(msg);
    if (len == 0 || len > MAX_MSG_SIZE)
        return EXIT_SUCCESS;

    pthread_mutex_lock(&mutex);
    if (log_entries >= MAX_LOG_ENTRIES)
    {
        pthread_mutex_unlock(&mutex);
        write_log();
        pthread_mutex_lock(&mutex);

    }

    struct tm* t = get_time_safe();
    if (!t)
    {
        pthread_mutex_unlock(&mutex);
        return EXIT_FAILURE;
    }

    snprintf(log_buffer[log_entries],
             MAX_ENTRY_SIZE,
             "%02d:%02d:%02d, %c, %s\n",
             t->tm_hour,
             t->tm_min,
             t->tm_sec,
             level,
             msg);

    log_entries++;

    pthread_mutex_unlock(&mutex);

    return EXIT_SUCCESS;
}

// WRITE LOG (ROTATION)

int write_log(void)
{
    char filename[LOG_FILENAME_MAX];
    build_log_filename(filename, sizeof(filename));

    pthread_mutex_lock(&mutex);

    FILE* f = fopen(filename, "a");
    if (!f)
    {
        pthread_mutex_unlock(&mutex);
        return EXIT_FAILURE;
    }

    for (uint16_t i = 0; i < log_entries; i++)
    {
        if (fprintf(f, "%s", log_buffer[i]) < 0)
        {
            fclose(f);
            pthread_mutex_unlock(&mutex);
            return EXIT_FAILURE;
        }
    }

    fclose(f);

    _clear_buffer();

    pthread_mutex_unlock(&mutex);

    return EXIT_SUCCESS;
}

// TERMINATION

void terminate_logging(void)
{
    write_log();
    disable_logging();
    _clear_buffer();
}
