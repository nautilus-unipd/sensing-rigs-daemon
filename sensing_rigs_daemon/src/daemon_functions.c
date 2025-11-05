// TODO: internal error handling

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

bool pid_check_file(void)
{
    FILE* restrict pid_file = fopen(DAEMON_PATH_PID, "r");
    if(pid_file == NULL)
    {
        return false;
    }
    char c = fgetc(pid_file);
    fclose(pid_file);
    pid_file = NULL;
    if((c == EOF) || (c == '\0'))
    {
        return false;
    }
    errno = EEXIST;
    return true;
}

bool pid_create_file(void)
{
    FILE* restrict pid_file = fopen(DAEMON_PATH_PID, "w");
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
    if(flock(fileno(pid_file), LOCK_EX | LOCK_NB) < 0)
    {
        fclose(pid_file);
        pid_file = NULL;
        return false;
    }
    fclose(pid_file);
    pid_file = NULL;
    return true;
}

void pid_close_file(void)
{
    FILE* restrict pid_file = fopen(DAEMON_PATH_PID, "w");
    if(pid_file == NULL)
    {
        return;
    }
    if(flock(fileno(pid_file), LOCK_UN) < 0)
    {
        fclose(pid_file);
        pid_file = NULL;
        return;
    }
    if(write(fileno(pid_file), "\0", 1) < 0)
    {
        fclose(pid_file);
        pid_file = NULL;
        return;
    }
    fclose(pid_file);
    pid_file = NULL;
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

    // Check for already running instances of this daemon
    if(pid_check_file())
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }
    if(!pid_create_file())
    {
        perror(ERR_DAEMON_CREATE);
        exit(EXIT_FAILURE);
    }

    // Start logging system
    init_logging();
    append_log(INFO, MSG_DAEMON_STARTED);

/*
    // Close every open file descriptor
    _close_all_fds();

    // Close STDIN and redirect STDOUT and STDERR to default file
    close(STDIN_FILENO);
    dup2(STDIN_FILENO, STDOUT_FILENO);
    dup2(STDIN_FILENO, STDERR_FILENO);
*/
    return;
}

void daemon_terminate(void)
{
    // Close PID file
    pid_close_file();

    // Terminate logging system
    append_log(INFO, MSG_DAEMON_KILLED);
    terminate_logging();

    // Close every open file descriptor
    _close_all_fds();
    return;
}
