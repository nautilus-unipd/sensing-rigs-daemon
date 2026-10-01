#include "fnct_runtime.h"

static unsigned long   g_seq[2] = {0, 0};
static pthread_mutex_t seq_lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned long highest_seq_in(const char* dir)
{
    DIR* d = opendir(dir);
    if(d == NULL)
    {
        return 0;
    }

    unsigned long hi = 0;
    struct dirent* e;
    while((e = readdir(d)) != NULL)
    {
        char* endp = NULL;
        unsigned long v = strtoul(e->d_name, &endp, 10);
        if(endp != e->d_name && (*endp == '_' || *endp == '.'))
        {
            if(v > hi)
            {
                hi = v;
            }
        }
    }

    closedir(d);
    return hi;
}

void init_seq_counters(void)
{
    char dir[512];

    snprintf(dir, sizeof(dir), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_RX);
    g_seq[0] = highest_seq_in(dir) + 1;

    snprintf(dir, sizeof(dir), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_LX);
    g_seq[1] = highest_seq_in(dir) + 1;
}

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
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    const char* subdir = (cam == 0) ? DAEMON_PATH_RX : DAEMON_PATH_LX;

    pthread_mutex_lock(&seq_lock);
    unsigned long seq = g_seq[cam]++;
    pthread_mutex_unlock(&seq_lock);

    char out_path[512];
    snprintf(out_path, sizeof(out_path), "%s%s%s%06lu_%ld.jpg", DAEMON_PATH, DAEMON_PATH_CAP, subdir, seq, (long)time(NULL));

    size_t cmd_size = strlen(PHOTO_SHOOT_COMMAND) + strlen(out_path) + 16;
    char* restrict command = (char*)malloc(cmd_size);
    if(command == NULL)
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    if(snprintf(command, cmd_size, PHOTO_SHOOT_COMMAND, cam, out_path) < 0)
    {
        free(command);
        command = NULL;
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    FILE* pipe = popen(command, "r");
    free(command);
    command = NULL;
    if(pipe == NULL)
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    pclose(pipe);
    return NULL;
}
