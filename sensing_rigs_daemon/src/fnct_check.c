#include "fnct_check.h"

bool check_space(void)
{
    struct statvfs stat;
    if(statvfs(ROOT_PATH, &stat) != 0)
    {
        return true;
    }
    if((stat.f_bfree * stat.f_frsize) <= MIN_FREE_SPACE)
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
