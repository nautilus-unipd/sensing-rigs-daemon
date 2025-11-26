#include "logger.h"
#include "messages.h"
#include "signal_handler.h"
#include "daemon_functions.h"

int main(void)
{
    // Initialise daemon
	daemon_create();

    // Daemon's main loop
    bool flag_run = true;
	while(flag_run)
	{
        if(block_signals() != EXIT_SUCCESS)
        {
            append_log(ERROR, ERR_BLOCK_SIGNALS);
            flag_run = false;
            daemon_terminate();
            return EXIT_FAILURE;
        }
        //sleep(5);
        //flag_run = false;
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
