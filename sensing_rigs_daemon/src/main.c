#include "logger.h"
#include "messages.h"
#include "signal_handler.h"
#include "daemon_functions.h"

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

    // Daemon's main loop
	while(flag_run)
	{
        if(block_signals() != EXIT_SUCCESS)
        {
            append_log(ERROR, ERR_BLOCK_SIGNALS);
            flag_run = false;
            daemon_terminate();
            return EXIT_FAILURE;
        }
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
	}

    // Terminate daemon
    daemon_terminate();
	return EXIT_SUCCESS;
}
