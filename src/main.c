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

	// Daemon's main loop
	// Loop description:
	//   1) Block signals
	//   2) Undervoltage check
	//   3) Not enough space left check
	//   4) Overheating check
	//   5) Cameras check
	//   6) Exit loop if previous checks fail
	//   7) Acquire images
	//   8) Sava images
	//   9) Unblock signals and handle the received ones, if any
	//  10) Repeat
	while(IS_FLAG(FLAG_RUN))
	{
		if(block_signals() != EXIT_SUCCESS)
		{
			append_log(ERROR, ERR_BLOCK_SIGNALS);
			CLEAR_FLAG(FLAG_RUN);
			daemon_terminate(true);
			return EXIT_FAILURE;
		}

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

		if(unblock_signals() != EXIT_SUCCESS)
		{
			append_log(ERROR, ERR_UNBLOCK_SIGNALS);
			CLEAR_FLAG(FLAG_RUN);
			daemon_terminate(true);
			return EXIT_FAILURE;
		}

        if ((IS_FLAG(FLAG_RCVD_SIGALRM) && IS_FLAG(FLAG_IMG_ACQ)))
        {
            pthread_t thread1;
            pthread_t thread2;
            int result1;
            int result2;
            result1 = pthread_create(&thread1, NULL, shoot, &cam0);
            if(result1 != 0)
            {
                append_log(ERROR, ERR_CREATE_THREAD);
                CLEAR_FLAG(FLAG_RUN);
                daemon_terminate(true);
                return EXIT_FAILURE;
            }
            result2 = pthread_create(&thread2, NULL, shoot, &cam1);
            if(result2 != 0)
            {
                append_log(ERROR, ERR_CREATE_THREAD);
                CLEAR_FLAG(FLAG_RUN);
                daemon_terminate(true);
                return EXIT_FAILURE;
            }
            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            CLEAR_FLAG(FLAG_RCVD_SIGALRM);
			
            if(IS_FLAG(FLAG_ERR_CAMS))
            {
                append_log(ERROR, ERR_CAMERAS);
                CLEAR_FLAG(FLAG_RUN);
                daemon_terminate(true);
                return EXIT_FAILURE;
            }

        }
    }

    // Terminate daemon
    daemon_terminate(true);
    return EXIT_SUCCESS;
}
