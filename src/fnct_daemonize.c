#include "fnct_daemonize.h"

FILE* open_pid_file(bool flag_write)
{
    char* restrict path_pid = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_PID) + 1));
    if(path_pid == NULL)
    {
        return NULL;
    }

    path_pid[0] = '\0';
    strcpy(path_pid, DAEMON_PATH);
    strcat(path_pid, DAEMON_PATH_PID);

    FILE* pid_file = NULL;

    if(flag_write)
    {
        pid_file = open_file(path_pid, FM_W);
    }
    else
    {
        pid_file = open_file(path_pid, FM_R);
    }

    free(path_pid);
    path_pid = NULL;

    return pid_file;
}

bool check_pid_file(void)
{
    FILE* restrict pid_file = open_pid_file(false);

    if(pid_file == NULL)
    {
        return false;
    }

    int c = fgetc(pid_file);
    fclose(pid_file);
    pid_file = NULL;

    if(c == EOF || c == '\0')
    {
        return false;
    }

    return false;
}

bool create_pid_file(void)
{
    FILE* restrict pid_file = open_pid_file(true);

    if(pid_file == NULL)
    {
        return false;
    }

    if(fprintf(pid_file, "%d", getpid()) <= 0)
    {
        fclose(pid_file);
        pid_file = NULL;
        return false;
    }

    fflush(pid_file);

    fclose(pid_file);
    pid_file = NULL;

    return true;
}

void close_pid_file(void)
{
    FILE* restrict pid_file = open_pid_file(true);

    if(pid_file == NULL)
    {
        return;
    }

    fflush(pid_file);
    ftruncate(fileno(pid_file), 0);

    fclose(pid_file);
    pid_file = NULL;
}

int daemon_create(void)
{

    if (mkdir(DAEMON_PATH, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_log[256];
    snprintf(path_log, sizeof(path_log), "%s%s", DAEMON_PATH, DAEMON_PATH_LOG);

    if (mkdir(path_log, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_img[256];
    snprintf(path_img, sizeof(path_img), "%s%s", DAEMON_PATH, DAEMON_PATH_CAP);

    if (mkdir(path_img, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_img_rx[256];
    snprintf(path_img_rx, sizeof(path_img_rx), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_RX);

    if (mkdir(path_img_rx, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_img_lx[256];
    snprintf(path_img_lx, sizeof(path_img_lx), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_LX);

    if (mkdir(path_img_lx, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    init_seq_counters();

    if (init_logging() != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    if (append_log(INFO, MSG_DAEMON_STARTED) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    if (init_sig_handler() != EXIT_SUCCESS)
    {
        append_log(ERROR, ERR_INIT_SH);
        return EXIT_FAILURE;
    }

    struct itimerval timer_shoot;

    timer_shoot.it_value.tv_sec = INTERVAL_SHOOT;
    timer_shoot.it_value.tv_usec = 0;

    timer_shoot.it_interval.tv_sec = INTERVAL_SHOOT;
    timer_shoot.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer_shoot, NULL);

    return EXIT_SUCCESS;
}

void daemon_terminate(bool close_pid)
{
    if(close_pid)
    {
        append_log(INFO, MSG_DAEMON_KILLED);
        close_pid_file();
    }

    terminate_logging();

    close_all_fds();

    return;
}
