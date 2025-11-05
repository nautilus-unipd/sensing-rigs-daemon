#include "logger.h"

struct LogBuffer_t
{
    bool active;
    uint8_t size;
    char* restrict buffer;
};

static struct LogBuffer_t log_buffer;

static pthread_mutex_t mutex_lb = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_lf = PTHREAD_MUTEX_INITIALIZER;

void _clear_buffer(void)
{
    pthread_mutex_lock(&mutex_lb);
    for(uint8_t i = 0; i < MAX_BUFF_DIM; i++)
    {
        *(log_buffer.buffer + i) = 0;
    }
    log_buffer.size = 0;
    pthread_mutex_unlock(&mutex_lb);
    return;
}

void init_logging(void)
{
    pthread_mutex_lock(&mutex_lb);
    log_buffer.active = true;
    log_buffer.size = 0;
    log_buffer.buffer = (char*)calloc(MAX_BUFF_DIM, sizeof(char));
    pthread_mutex_unlock(&mutex_lb);
    if(log_buffer.buffer == NULL)
    {
        // TODO: internal error handling
        exit(EXIT_FAILURE);
    }
    return;
}

void disable_logging(void)
{
    pthread_mutex_lock(&mutex_lb);
    log_buffer.active = false;
    pthread_mutex_unlock(&mutex_lb);
    return;
}

void enable_logging(void)
{
    pthread_mutex_lock(&mutex_lb);
    log_buffer.active = true;
    pthread_mutex_unlock(&mutex_lb);
    return;
}

void append_log(enum LogLevel_t level, const char* restrict msg)
{
    if((!msg) || (!log_buffer.active) || (strlen(msg) <= 1) || (strlen(msg) > MAX_MSG_SIZE))
    {
        return;
    }
    time_t* restrict time_raw = (time_t*)malloc(sizeof(time_t));
    if(time_raw == NULL)
    {
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    *(time_raw) = time(NULL);
    struct tm* restrict time_local = localtime(time_raw);
    if(time_local == NULL)
    {
        free(time_raw);
        time_raw = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    char* restrict real_msg = (char*)calloc(MAX_BUFF_DIM, sizeof(char));
    if(real_msg == NULL)
    {
        free(time_raw);
        time_raw = NULL;
        time_local = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    if(snprintf(real_msg, MAX_BUFF_DIM, BASE_LOG_ENTRY, time_local->tm_hour, time_local->tm_min, time_local->tm_sec, level, msg) < 0)
    {
        free(time_raw);
        time_raw = NULL;
        time_local = NULL;
        free(real_msg);
        real_msg = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    if(log_buffer.size + strlen(real_msg) + 1 > MAX_BUFF_DIM)
    {
        write_log(); 
    }
    pthread_mutex_lock(&mutex_lb);
    if(strncpy(log_buffer.buffer + log_buffer.size, real_msg, strlen(real_msg)) == NULL)
    {
        pthread_mutex_unlock(&mutex_lb);
        free(time_raw);
        time_raw = NULL;
        time_local = NULL;
        free(real_msg);
        real_msg = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    log_buffer.size += strlen(real_msg);
    pthread_mutex_unlock(&mutex_lb);
    free(time_raw);
    time_raw = NULL;
    time_local = NULL;
    free(real_msg);
    real_msg = NULL;
    return;
}

void write_log(void)
{
    char* restrict full_path_log = (char*)malloc(strlen(DAEMON_PATH_LOG));
    if(full_path_log == NULL)
    {
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    time_t* restrict time_raw = (time_t*)malloc(sizeof(time_t));
    if(time_raw == NULL)
    {
        // TODO: internal error
        free(full_path_log);
        full_path_log = NULL;
        exit(EXIT_FAILURE);
    }
    *(time_raw) = time(NULL);
    struct tm* restrict time_local = localtime(time_raw);
    if(time_local == NULL)
    {
        free(full_path_log);
        full_path_log = NULL;
        free(time_raw);
        time_raw = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    if(snprintf(full_path_log, strlen(DAEMON_PATH_LOG), DAEMON_PATH_LOG, time_local->tm_year + 1900, time_local->tm_mon + 1, time_local->tm_mday) < 0)
    {
        free(full_path_log);
        full_path_log = NULL;
        free(time_raw);
        time_raw = NULL;
        time_local = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    free(time_raw);
    time_raw = NULL;
    time_local = NULL;
    pthread_mutex_lock(&mutex_lf);
    FILE* restrict log_file = fopen(full_path_log, "a");
    if(log_file == NULL)
    {
        pthread_mutex_unlock(&mutex_lf);
        free(full_path_log);
        full_path_log = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    if(write(fileno(log_file), log_buffer.buffer, MAX_BUFF_DIM) < 0)
    {
        fclose(log_file);
        log_file = NULL;
        pthread_mutex_unlock(&mutex_lf);
        free(full_path_log);
        full_path_log = NULL;
        // TODO: internal error
        exit(EXIT_FAILURE);
    }
    fclose(log_file);
    log_file = NULL;
    pthread_mutex_unlock(&mutex_lf);
    free(full_path_log);
    full_path_log = NULL;
    _clear_buffer();
    return;
}

void terminate_logging(void)
{
    write_log();
    disable_logging();
    pthread_mutex_lock(&mutex_lb);
    free(log_buffer.buffer);
    log_buffer.buffer = NULL;
    pthread_mutex_unlock(&mutex_lb);
    return;
}
