#include "logger.h"
#include "messages.h"
#include "fnct_helper.h"
#include "fnct_runtime.h"
#include "signal_handler.h"
#include "fnct_daemonize.h"

int main(void)
{
	// Initialize daemon
	if(daemon_create() != EXIT_SUCCESS)
	{
		daemon_terminate(false);
		exit(EXIT_FAILURE);
	}

	// Initialize some variables
	init_daemon_flags();

	uint8_t cam0 = 0;
	uint8_t cam1 = 1;

	// Initial health checks before starting acquisition
	if(check_voltage())
	{
		append_log(ERROR, ERR_UNDERVOLTAGE);
		CLEAR_FLAG(FLAG_RUN);
		daemon_terminate(true);
		return EXIT_FAILURE;
	}

	if(check_temperature())
	{
		append_log(ERROR, ERR_OVERHEATING);
		CLEAR_FLAG(FLAG_RUN);
		daemon_terminate(true);
		return EXIT_FAILURE;
	}

	if(check_space())
	{
		append_log(ERROR, ERR_SPACE_FULL);
		CLEAR_FLAG(FLAG_RUN);
		daemon_terminate(true);
		return EXIT_FAILURE;
	}

	uint8_t health_check_counter = 0;

	// Daemon's main loop:
	// Sleeps via pause() until SIGALRM arrives (every 200 ms -> 5 FPS).
	// Triggers captures instantaneously via SIGUSR1 to persistent rpicam-still processes.
	while(IS_FLAG(FLAG_RUN))
	{
		if(!IS_FLAG(FLAG_RCVD_SIGALRM))
		{
			pause();
		}

		if(!IS_FLAG(FLAG_RUN))
		{
			break;
		}

		if(IS_FLAG(FLAG_RCVD_SIGALRM))
		{
			CLEAR_FLAG(FLAG_RCVD_SIGALRM);

			// 1. Trigger captures immediately with zero latency
			if(IS_FLAG(FLAG_IMG_ACQ))
			{
				trigger_camera(cam0);
				trigger_camera(cam1);

				if(IS_FLAG(FLAG_ERR_CAMS))
				{
					append_log(ERROR, ERR_CAMERAS);
					CLEAR_FLAG(FLAG_RUN);
					daemon_terminate(true);
					return EXIT_FAILURE;
				}
			}

			// 2. Periodic health check every 50 frames (~10 seconds at 5 FPS)
			// Runs AFTER camera triggers so it does not introduce any trigger jitter!
			if(++health_check_counter >= 50)
			{
				health_check_counter = 0;

				if(check_voltage())
				{
					append_log(ERROR, ERR_UNDERVOLTAGE);
					CLEAR_FLAG(FLAG_RUN);
					daemon_terminate(true);
					return EXIT_FAILURE;
				}

				if(check_temperature())
				{
					append_log(ERROR, ERR_OVERHEATING);
					CLEAR_FLAG(FLAG_RUN);
					daemon_terminate(true);
					return EXIT_FAILURE;
				}

				if(check_space())
				{
					append_log(ERROR, ERR_SPACE_FULL);
					CLEAR_FLAG(FLAG_RUN);
					daemon_terminate(true);
					return EXIT_FAILURE;
				}
			}
		}
	}

    // Terminate daemon
    daemon_terminate(true);
    return EXIT_SUCCESS;
}
