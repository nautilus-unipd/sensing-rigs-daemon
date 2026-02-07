#include "fnct_helper.h"

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

FILE* open_file(const char* restrict file_path, const char* restrict mode)
{
	FILE* new_fptr = fopen(file_path, mode);
	if(new_fptr == NULL)
	{
		return NULL;
	}
	return new_fptr;
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
