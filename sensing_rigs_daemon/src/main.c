#include "logger.h"
#include "messages.h"
#include "fnct_check.h"
#include "signal_handler.h"
#include "fnct_daemonize.h"

int main(void)
{
    // Initialize daemon
	if(daemon_create() != EXIT_SUCCESS)
    {
        daemon_terminate();
        exit(EXIT_FAILURE);
    }

    // Initialize some variables
    bool flag_run = true;
    bool flag_img_acq = true;
    /*
    bool flag_undervoltage = false;
    bool flag_overheating = false;
    bool flag_space_full = false;
    // TODO: group these into an unsigned char? (1 bit per flag)
    */

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
            daemon_terminate();
            return EXIT_FAILURE;
        }

        // TODO: needed only on the Raspberry PI 5
/*
        if(check_voltage())
        {
            append_log(ERROR, ERR_UNDERVOLTAGE);
            flag_run = false;
            daemon_terminate();
            return EXIT_FAILURE;
        }

        if(check_temperature())
        {
            append_log(ERROR, ERR_OVERHEATING);
            flag_run = false;
            daemon_terminate();
            return EXIT_FAILURE;
        }

        if(check_space())
        {
            append_log(ERROR, ERR_SPACE_FULL);
            flag_run = false;
            daemon_terminate();
            return EXIT_FAILURE;
        }
*/

        if(flag_img_acq)
        {
            sleep(3);
        }

        if(unblock_signals() != EXIT_SUCCESS)
        {
            append_log(ERROR, ERR_UNBLOCK_SIGNALS);
            flag_run = false;
            daemon_terminate();
            return EXIT_FAILURE;
        }
        if(rcvd_sighup)
        {
            rcvd_sighup = false;
            if(flag_img_acq)
            {
                flag_img_acq = false;
                if(append_log(INFO, MSG_RCVD_SIGHUP1) != EXIT_SUCCESS)
                {
                    flag_run = false;
                    daemon_terminate();
                    return EXIT_FAILURE;
                }
            }
            else
            {
                flag_img_acq = true;
                if(append_log(INFO, MSG_RCVD_SIGHUP2) != EXIT_SUCCESS)
                {
                    flag_run = false;
                    daemon_terminate();
                    return EXIT_FAILURE;
                }
            }
        }
	}

    // Terminate daemon
    daemon_terminate();
	return EXIT_SUCCESS;
}
