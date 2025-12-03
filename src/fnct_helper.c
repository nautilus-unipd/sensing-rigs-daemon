#include "fnct_helper.h"

bool volatile flag_err_cams = false;

void close_all_fds(void)
{
	int max_fd = sysconf(_SC_OPEN_MAX);
	if(max_fd == -1)
	{
		max_fd = MAX_FDS;
	}
	for(int fd = 0; fd < max_fd; fd++)
	{
		close(fd);
	}
	return;
}

struct tm* get_local_time(void)
{
	time_t* restrict time_raw = (time_t*)malloc(sizeof(time_t));
	if(time_raw == NULL)
	{
		return NULL;
	}
	*(time_raw) = time(NULL);
	struct tm* time_local = localtime(time_raw);
	if(time_local == NULL)
	{
		free(time_raw);
		time_raw = NULL;
		return NULL;
	}
	free(time_raw);
	time_raw = NULL;
	return time_local;
}

void* shoot(void* arg)
{
	uint8_t cam = *(uint8_t*)arg;
	if((cam != 0) && (cam != 1))
	{
		flag_err_cams = true;
		return NULL;
	}
	char* restrict command = (char*)malloc(sizeof(char) * MAX_COMMAND_SIZE);
	if(command == NULL)
	{
		flag_err_cams = true;
		return NULL;
	}
	struct tm* time_local = get_local_time();
	if(time_local == NULL)
	{
		free(command);
		command = NULL;
		flag_err_cams = true;
		return NULL;
	}
	if(cam == 0)
	{
		if(snprintf(command, MAX_COMMAND_SIZE, PHOTO_SHOOT_COMMAND, cam, DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_RX, time_local->tm_hour, time_local->tm_min, time_local->tm_sec) < 0)
		{
			free(command);
			command = NULL;
			time_local = NULL;
			flag_err_cams = true;
			return NULL;
		}
	}
	else
	{
		if(snprintf(command, MAX_COMMAND_SIZE, PHOTO_SHOOT_COMMAND, cam, DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_LX, time_local->tm_hour, time_local->tm_min, time_local->tm_sec) < 0)
		{
			free(command);
			command = NULL;
			time_local = NULL;
			flag_err_cams = true;
			return NULL;
		}
	}
	FILE *pipe = popen(command, "r");
	free(command);
	command = NULL;
	time_local = NULL;
	if(pipe == NULL)
	{
		flag_err_cams = true;
		return NULL;
	}
	pclose(pipe);
	return NULL;
}
