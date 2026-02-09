#include "signal_handler.h"

//                          1       2       3        6        10       12       14       15       20
const uint8_t BLCK_SIG[] = {SIGHUP, SIGINT, SIGQUIT, SIGABRT, SIGUSR1, SIGUSR2, SIGALRM, SIGTERM, SIGTSTP};

static pthread_mutex_t mutex_mask = PTHREAD_MUTEX_INITIALIZER;

static sigset_t block_mask;
static sigset_t old_mask;

bool volatile flag_img_acq = true;
bool volatile rcvd_sigalrm = false;

void sig_handler(int signo)
{
	if(signo == SIGTERM)
	{
		append_log(ERROR, ERR_RCVD_SIGTERM);
		daemon_terminate(true);
		exit(EXIT_FAILURE);
	}
	else if(signo == SIGABRT)
	{
		append_log(ERROR, ERR_RCVD_SIGABRT);
		daemon_terminate(true);
		exit(EXIT_FAILURE);
	}
	else if(signo == SIGHUP)
	{
		if(flag_img_acq)
		{
			append_log(INFO, MSG_RCVD_SIGHUP1);
			flag_img_acq = false;
		}
		else
		{
			append_log(INFO, MSG_RCVD_SIGHUP2);
			flag_img_acq = true;
		}
	}
	else if(signo == SIGALRM)
	{
		if(flag_img_acq)
		{
			append_log(INFO, MSG_RCVD_SIGALRM);
			rcvd_sigalrm = true;
		}
	}
	return;
}

int init_sig_handler(void)
{
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	for(uint8_t i = 0; i < sizeof(BLCK_SIG); i++)
	{
		if(sigaction(BLCK_SIG[i], &sa, NULL) != 0)
		{
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int block_signals(void)
{
	pthread_mutex_lock(&mutex_mask);
	sigemptyset(&block_mask);
	for(uint8_t i = 0; i < sizeof(BLCK_SIG); i++)
	{
		if(sigaddset(&block_mask, BLCK_SIG[i]) != 0)
		{
			pthread_mutex_unlock(&mutex_mask);
			return EXIT_FAILURE;
		}
	}
	if(sigprocmask(SIG_BLOCK, &block_mask, &old_mask) != 0)
	{
		pthread_mutex_unlock(&mutex_mask);
		return EXIT_FAILURE;
	}
	pthread_mutex_unlock(&mutex_mask);
	return EXIT_SUCCESS;
}

int unblock_signals(void)
{
	pthread_mutex_lock(&mutex_mask);
	if(sigprocmask(SIG_SETMASK, &old_mask, NULL) != 0)
	{
		pthread_mutex_unlock(&mutex_mask);
		return EXIT_FAILURE;
	}
	pthread_mutex_unlock(&mutex_mask);
	return EXIT_SUCCESS;
}
