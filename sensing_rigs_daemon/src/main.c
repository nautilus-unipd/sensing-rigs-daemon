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
	bool flag_run = true;

/* Unused since this is a development version
	uint8_t cam0 = 0;
	uint8_t cam1 = 1;
*/
	//bool flag_undervoltage = false;
	//bool flag_overheating = false;
	//bool flag_space_full = false;

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
	while(flag_run)
	{
		if(block_signals() != EXIT_SUCCESS)
		{
			append_log(ERROR, ERR_BLOCK_SIGNALS);
			flag_run = false;
			daemon_terminate(true);
			return EXIT_FAILURE;
		}

/* Unused since this is a development version
		if(check_voltage())
		{
			append_log(ERROR, ERR_UNDERVOLTAGE);
			flag_run = false;
			daemon_terminate(true);
			return EXIT_FAILURE;
		}

		if(check_temperature())
		{
			append_log(ERROR, ERR_OVERHEATING);
			flag_run = false;
			daemon_terminate(true);
			return EXIT_FAILURE;
		}

		if(check_space())
		{
			append_log(ERROR, ERR_SPACE_FULL);
			flag_run = false;
			daemon_terminate(true);
			return EXIT_FAILURE;
		}
*/
		if(unblock_signals() != EXIT_SUCCESS)
		{
			append_log(ERROR, ERR_UNBLOCK_SIGNALS);
			flag_run = false;
			daemon_terminate(true);
			return EXIT_FAILURE;
		}

        if(rcvd_sigalrm && flag_img_acq)
        {
            rcvd_sigalrm = false;
/* Unused since this is a development version
            pthread_t thread1;
            pthread_t thread2;
            int result1;
            int result2;
            result1 = pthread_create(&thread1, NULL, shoot, &cam0);
            if(result1 != 0)
            {
                append_log(ERROR, ERR_CREATE_THREAD);
                flag_run = false;
                daemon_terminate(true);
                return EXIT_FAILURE;
            }
            result2 = pthread_create(&thread2, NULL, shoot, &cam1);
            if(result2 != 0)
            {
                append_log(ERROR, ERR_CREATE_THREAD);
                flag_run = false;
                daemon_terminate(true);
                return EXIT_FAILURE;
            }
            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            if(flag_err_cams)
            {
                append_log(ERROR, ERR_CAMERAS);
                flag_run = false;
                daemon_terminate(true);
                return EXIT_FAILURE;
            }
*/
        }
    }

    // Terminate daemon
    daemon_terminate(true);
    return EXIT_SUCCESS;
}
