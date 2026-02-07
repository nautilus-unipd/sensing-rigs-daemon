#include "fnct_runtime.h"

bool volatile flag_err_cams = false;

bool check_space(void)
{
    struct statvfs stat;
    if(statvfs(ROOT_PATH, &stat) != 0)
    {
        return true;
    }
    if((stat.f_bfree * stat.f_frsize) <= ((unsigned long int)MIN_FREE_SPACE))
    {
        return true;
    }
    return false;
}

bool check_temperature(void)
{
    FILE* temp_file = fopen(FILE_PATH_TEMP, "r");
    if(temp_file == NULL)
    {
        return true;
    }
    uint32_t temp = 0;
    if(fscanf(temp_file, "%u", &temp) == EOF)
    {
        fclose(temp_file);
        return true;
    }
    fclose(temp_file);
    if(temp >= MAX_TEMP)
    {
        return true;
    }
    return false;
}

bool check_voltage(void)
{
    FILE* volt_pipe;
    volt_pipe = popen(VOLT_COMMAND, "r");
    if(volt_pipe == NULL)
    {
        return true;
    }
    char* result = (char*)malloc(MAX_VOLT_BUFF_SIZE * sizeof(char));
    if(result == NULL)
    {
        pclose(volt_pipe);
        return true;
    }
    if(fgets(result, MAX_VOLT_BUFF_SIZE * sizeof(char), volt_pipe) == NULL)
    {
        free(result);
        result = NULL;
        pclose(volt_pipe);
        return true;
    }
    pclose(volt_pipe);
    if((*(result) != VOLT_CORRECT_1) || (*(result + 1) != VOLT_CORRECT_2))
    {
        free(result);
        result = NULL;
        return true;
    }
    free(result);
    result = NULL;
    return false;
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
	if(cam == 0)
	{
		if(snprintf(command, MAX_COMMAND_SIZE, PHOTO_SHOOT_COMMAND, cam, DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_RX) < 0)
		{
			free(command);
			command = NULL;
			flag_err_cams = true;
			return NULL;
		}
	}
	else
	{
		if(snprintf(command, MAX_COMMAND_SIZE, PHOTO_SHOOT_COMMAND, cam, DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_LX) < 0)
		{
			free(command);
			command = NULL;
			flag_err_cams = true;
			return NULL;
		}
	}
	FILE *pipe = popen(command, "r");
	free(command);
	command = NULL;
	if(pipe == NULL)
	{
		flag_err_cams = true;
		return NULL;
	}
	pclose(pipe);
	return NULL;
}
