#include "logger.h"

bool volatile log_active = true;
uint8_t volatile log_entries = 0;

struct LogBuffer_t
{
	char msg[MAX_ENTRY_SIZE];
};

struct LogBuffer_t log_buffer[MAX_LOG_ENTRIES];

static pthread_mutex_t mutex_lb = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_lf = PTHREAD_MUTEX_INITIALIZER;

void _clear_buffer(void)
{
	pthread_mutex_lock(&mutex_lb);
	for(uint8_t i = 0; i < MAX_LOG_ENTRIES; i++)
	{
        strncpy((log_buffer + i)->msg, "", MAX_ENTRY_SIZE);
	}
    log_entries = 0;
	pthread_mutex_unlock(&mutex_lb);
	return;
}

void internal_error(void)
{
    terminate_logging();
	return;
}

int init_logging(void)
{
	pthread_mutex_lock(&mutex_lb);
	log_active = true;
	log_entries = 0;
	pthread_mutex_unlock(&mutex_lb);
	return EXIT_SUCCESS;
}

void disable_logging(void)
{
	pthread_mutex_lock(&mutex_lb);
	log_active = false;
	pthread_mutex_unlock(&mutex_lb);
	return;
}

void enable_logging(void)
{
	pthread_mutex_lock(&mutex_lb);
	log_active = true;
	pthread_mutex_unlock(&mutex_lb);
	return;
}

int append_log(enum LogLevel_t level, const char* restrict msg)
{
	if((!msg) || (!log_active) || (strlen(msg) <= 1) || (strlen(msg) > MAX_MSG_SIZE))
	{
		return EXIT_SUCCESS;
	}
	char* restrict real_msg = (char*)calloc(MAX_ENTRY_SIZE, sizeof(char));
	if(real_msg == NULL)
	{
		internal_error();
		return EXIT_FAILURE;
	}
	struct tm* time_local = get_local_time();
	if(time_local == NULL)
	{
		internal_error();
		return EXIT_FAILURE;
	}
	if(snprintf(real_msg, MAX_ENTRY_SIZE, BASE_LOG_ENTRY, time_local->tm_hour, time_local->tm_min, time_local->tm_sec, level, msg) < 0)
	{
		time_local = NULL;
		free(real_msg);
		real_msg = NULL;
		internal_error();
		return EXIT_FAILURE;
	}
	if((log_entries + 1) >= MAX_LOG_ENTRIES)
	{
		if(write_log() != EXIT_SUCCESS)
		{
			internal_error();
			return EXIT_FAILURE;
		}
	}
	time_local = NULL;
	pthread_mutex_lock(&mutex_lb);
	if(strncpy((log_buffer + log_entries)->msg, real_msg, strlen(real_msg)) == NULL)
	{
		pthread_mutex_unlock(&mutex_lb);
		free(real_msg);
		real_msg = NULL;
		internal_error();
		return EXIT_FAILURE;
	}
    log_entries++;
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
	struct tm* time_local = get_local_time();
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
	FILE* log_file = open_file(real_file_log, FM_A);
	free(real_file_log);
	real_file_log = NULL;
	if(log_file == NULL)
	{
		pthread_mutex_unlock(&mutex_lf);
		internal_error();
		return EXIT_FAILURE;
	}
    for(uint8_t i = 0; i < MAX_LOG_ENTRIES; i++)
    {
        if(write(fileno(log_file), (log_buffer + i)->msg, MAX_ENTRY_SIZE) < 0)
        {
            fclose(log_file);
            log_file = NULL;
            pthread_mutex_unlock(&mutex_lf);
            internal_error();
            return EXIT_FAILURE;
        }
	}
	fclose(log_file);
	log_file = NULL;
	pthread_mutex_unlock(&mutex_lf);
	_clear_buffer();
	return EXIT_SUCCESS;
}

void terminate_logging(void)
{
	write_log();
	disable_logging();
    _clear_buffer();
	return;
}
