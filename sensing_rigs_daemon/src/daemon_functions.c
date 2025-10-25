#include "daemon_functions.h"

void _close_all_fds(void)
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

void daemon_create(void)
{
    // Create child process and check for errors
	pid_t pid = fork();
	if(pid < 0)
	{
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
	}
	if(pid > 0)
	{
        _exit(EXIT_SUCCESS);
    }

    // Make child the session leader
	if(setsid() < 0)
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }

    // Modify parent's behaviour to certain signals:
    // SIGHCHLD     when child process terminates   ->  ignore
    // SIGHUP       when controlling terminal exits ->  ignore
	if(signal(SIGCHLD, SIG_IGN) == SIG_ERR)
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }
	if(signal(SIGHUP, SIG_IGN) == SIG_ERR)
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }

    // Create second child process and check for errors
	pid = fork();
	if(pid < 0)
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }
	if(pid > 0)
    {
        _exit(EXIT_SUCCESS);
    }

    // Set new umask value
    umask(S_IWGRP | S_IWOTH);

    // Change current working directory and check for errors
    if(chdir("/") < 0)
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }

    // Start logging system
    init_logging();

    // Close every open file descriptor
    _close_all_fds();

    // Close STDIN and redirect STDOUT and STDERR to default file
    close(STDIN_FILENO);
    int16_t fd = open(REDIRECT_FILE, O_RDWR);
    if(fd != STDIN_FILENO)
    {
        exit(EXIT_FAILURE);
    }
    if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
    {
        exit(EXIT_FAILURE);
    }
    if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
    {
        exit(EXIT_FAILURE);
    }

    // Write daemon pid to a file
    pid = getpid();
    char* restrict full_path_pid = (char*)malloc(strlen(DAEMON_BASE_PATH) + strlen(DAEMON_FILE_PID) + 1);
    strcpy(full_path_pid, DAEMON_BASE_PATH);
    strcat(full_path_pid, DAEMON_FILE_PID);
    FILE* restrict pid_file = fopen(full_path_pid, "w");
    if(pid_file == NULL)
    {
        free(full_path_pid);
        full_path_pid = NULL;
        exit(EXIT_FAILURE);
    }
    fprintf(pid_file, "%d", pid);
    fclose(pid_file);
    pid_file = NULL;
    free(full_path_pid);
    full_path_pid = NULL;
    return;
}

void daemon_terminate(void)
{
    // Terminate logging system
    terminate_logging();

    // Close every open file descriptor
    _close_all_fds();
    return;
}
