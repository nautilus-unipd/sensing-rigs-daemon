#include "fnct_daemonize.h"

FILE* open_pid_file(bool flag_write)
{
	char* restrict path_pid = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_PID) + 1));
	if(path_pid == NULL)
	{
		return NULL;
	}
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
	char c = fgetc(pid_file);
	fclose(pid_file);
	pid_file = NULL;
	if(c == '\0')
	{
		return false;
	}
	errno = EEXIST;
	return true;
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

void close_pid_file(void)
{
	FILE* restrict pid_file = open_pid_file(true);
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

int daemon_create(void)
{
	// Create child process and check for errors
	pid_t pid = fork();
	if(pid < 0)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	if(pid > 0)
	{
		_exit(EXIT_SUCCESS);
	}

	// Make child the session leader
	if(setsid() < 0)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}

	// Modify parent's behaviour to certain signals:
	// SIGHCHLD     when child process terminates   ->  ignore
	// SIGHUP       when controlling terminal exits ->  ignore
	if(signal(SIGCHLD, SIG_IGN) == SIG_ERR)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	if(signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}

	// Create second child process and check for errors
	pid = fork();
	if(pid < 0)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	if(pid > 0)
	{
		_exit(EXIT_SUCCESS);
	}

	// Set new umask value
	umask(S_IWGRP | S_IWOTH);

	// Change current working directory and check for errors
	if(chdir(ROOT_PATH) < 0)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}

	// Create runtimes directories
	if((mkdir(DAEMON_PATH, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH)) != 0 && (errno != EEXIST))
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	char* restrict path_log = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_LOG) + 1));
	if(path_log == NULL)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	strcat(path_log, DAEMON_PATH);
	strcat(path_log, DAEMON_PATH_LOG);
	if((mkdir(path_log, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH) != 0) && (errno != EEXIST))
	{
		free(path_log);
		path_log = NULL;
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	free(path_log);
	path_log = NULL;
	char* restrict path_img = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_CAP) + 1));
	if(path_img == NULL)
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	strcat(path_img, DAEMON_PATH);
	strcat(path_img, DAEMON_PATH_CAP);
	if((mkdir(path_img, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH) != 0) && (errno != EEXIST))
	{
		free(path_img);
		path_img = NULL;
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	char* restrict path_img_rx = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_CAP) + strlen(DAEMON_PATH_RX) + 1));
	if(path_img_rx == NULL)
	{
		free(path_img);
		path_img = NULL;
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	char* restrict path_img_lx = (char*)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_CAP) + strlen(DAEMON_PATH_LX) + 1));
	if(path_img_lx == NULL)
	{
		free(path_img);
		path_img = NULL;
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	strcat(path_img_rx, path_img);
	strcat(path_img_rx, DAEMON_PATH_RX);
	strcat(path_img_lx, path_img);
	strcat(path_img_lx, DAEMON_PATH_LX);
	if((mkdir(path_img_rx, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH) != 0) && (errno != EEXIST))
	{
		free(path_img);
		path_img = NULL;
		free(path_img_rx);
		path_img_rx = NULL;
		free(path_img_lx);
		path_img_lx = NULL;
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	free(path_img_rx);
	path_img_rx = NULL;
	if((mkdir(path_img_lx, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH) != 0) && (errno != EEXIST))
	{
		free(path_img);
		path_img = NULL;
		free(path_img_lx);
		path_img_lx = NULL;
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	free(path_img_lx);
	path_img_lx = NULL;
	free(path_img);
	path_img = NULL;

	// Check for already running instances of this daemon
	if(check_pid_file())
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}
	if(!create_pid_file())
	{
		perror(ERR_DAEMON_CREATE);
		return EXIT_FAILURE;
	}

	// Close every open file descriptor
	close_all_fds();

	// Close STDIN and redirect STDOUT and STDERR to default file
	close(STDIN_FILENO);
	dup2(STDIN_FILENO, STDOUT_FILENO);
	dup2(STDIN_FILENO, STDERR_FILENO);

	// Start logging system
	if(init_logging() != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	if(append_log(INFO, MSG_DAEMON_STARTED) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	// Start signal handler
	if(init_sig_handler() != EXIT_SUCCESS)
	{
		append_log(ERROR, ERR_INIT_SH);
		return EXIT_FAILURE;
	}

    // Start timer
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
