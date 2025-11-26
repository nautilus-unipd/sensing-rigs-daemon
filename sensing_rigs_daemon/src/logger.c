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

void internal_error(void)
{
    if(log_buffer.buffer == NULL)
    {
        return;
    }
    pthread_mutex_lock(&mutex_lb);
    free(log_buffer.buffer);
    log_buffer.buffer = NULL;
    pthread_mutex_unlock(&mutex_lb);
    return;
}

struct tm* _get_local_time(void)
{
    time_t* restrict time_raw = (time_t*)malloc(sizeof(time_t));
    if(time_raw == NULL)
    {
        internal_error();
        return NULL;
    }
    *(time_raw) = time(NULL);
    struct tm* time_local = localtime(time_raw);
    if(time_local == NULL)
    {
        free(time_raw);
        time_raw = NULL;
        internal_error();
        return NULL;
    }
    free(time_raw);
    time_raw = NULL;
    return time_local;
}

int init_logging(void)
{
    pthread_mutex_lock(&mutex_lb);
    log_buffer.active = true;
    log_buffer.size = 0;
    log_buffer.buffer = (char*)calloc(MAX_BUFF_DIM, sizeof(char));
    pthread_mutex_unlock(&mutex_lb);
    if(log_buffer.buffer == NULL)
    {
        internal_error();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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

int append_log(enum LogLevel_t level, const char* restrict msg)
{
    if((!msg) || (!log_buffer.active) || (strlen(msg) <= 1) || (strlen(msg) > MAX_MSG_SIZE))
    {
        return EXIT_SUCCESS;
    }
    char* restrict real_msg = (char*)calloc(MAX_BUFF_DIM, sizeof(char));
    if(real_msg == NULL)
    {
        internal_error();
        return EXIT_FAILURE;
    }
    struct tm* time_local = _get_local_time();
    if(time_local == NULL)
    {
        internal_error();
        return EXIT_FAILURE;
    }
    if(snprintf(real_msg, MAX_BUFF_DIM, BASE_LOG_ENTRY, time_local->tm_hour, time_local->tm_min, time_local->tm_sec, level, msg) < 0)
    {
        time_local = NULL;
        free(real_msg);
        real_msg = NULL;
        internal_error();
        return EXIT_FAILURE;
    }
    if(log_buffer.size + strlen(real_msg) + 1 > MAX_BUFF_DIM)
    {
        if(write_log() != EXIT_SUCCESS)
        {
            internal_error();
            return EXIT_FAILURE;
        }
    }
    time_local = NULL;
    pthread_mutex_lock(&mutex_lb);
    if(strncpy(log_buffer.buffer + log_buffer.size, real_msg, strlen(real_msg)) == NULL)
    {
        pthread_mutex_unlock(&mutex_lb);
        free(real_msg);
        real_msg = NULL;
        internal_error();
        return EXIT_FAILURE;
    }
    log_buffer.size += strlen(real_msg);
    pthread_mutex_unlock(&mutex_lb);
    free(real_msg);
    real_msg = NULL;
    return EXIT_SUCCESS;
}

int write_log(void)
{
    char* restrict full_path_log = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_LOG) + strlen(BASE_LOG_FILE) + 1));
    if(full_path_log == NULL)
    {
        internal_error();
        return EXIT_FAILURE;
    }
    strcpy(full_path_log, DAEMON_PATH);
    strcat(full_path_log, DAEMON_PATH_LOG);
    strcat(full_path_log, BASE_LOG_FILE);
    char* restrict real_file_log = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_LOG) + strlen(BASE_LOG_FILE) + 1));
    if(real_file_log == NULL)
    {
        free(full_path_log);
        full_path_log = NULL;
        internal_error();
        return EXIT_FAILURE;
    }
    struct tm* time_local = _get_local_time();
    if(time_local == NULL)
    {
        internal_error();
        return EXIT_FAILURE;
    }
    if(snprintf(real_file_log, strlen(DAEMON_PATH) + strlen(DAEMON_PATH_LOG) + strlen(BASE_LOG_FILE) + 1, full_path_log, time_local->tm_year + 1900, time_local->tm_mon + 1, time_local->tm_mday) < 0)
    {
        free(real_file_log);
        real_file_log = NULL;
        free(full_path_log);
        full_path_log = NULL;
        time_local = NULL;
        internal_error();
        return EXIT_FAILURE;
    }
    free(full_path_log);
    full_path_log = NULL;
    time_local = NULL;
    pthread_mutex_lock(&mutex_lf);
    FILE* restrict log_file = fopen(real_file_log, "a");
    free(real_file_log);
    real_file_log = NULL;
    if(log_file == NULL)
    {
        pthread_mutex_unlock(&mutex_lf);
        internal_error();
        return EXIT_FAILURE;
    }
    if(write(fileno(log_file), log_buffer.buffer, MAX_BUFF_DIM) < 0)
    {
        fclose(log_file);
        log_file = NULL;
        pthread_mutex_unlock(&mutex_lf);
        internal_error();
        return EXIT_FAILURE;
    }
    fclose(log_file);
    log_file = NULL;
    pthread_mutex_unlock(&mutex_lf);
    _clear_buffer();
    return EXIT_SUCCESS;
}

void terminate_logging(void)
{
    if(write_log() != EXIT_SUCCESS)
    {
        internal_error();
        return;
    }
    disable_logging();
    pthread_mutex_lock(&mutex_lb);
    free(log_buffer.buffer);
    log_buffer.buffer = NULL;
    pthread_mutex_unlock(&mutex_lb);
    return;
}
