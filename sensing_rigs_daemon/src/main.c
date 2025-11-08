#include "logger.h"
#include "messages.h"
#include "daemon_functions.h"

int main(void)
{
    // Initialise daemon
	daemon_create();

    // Daemon's main loop
    bool flag_run = true;
	while(flag_run)
	{
        sleep(3);
        flag_run = false;
	}

    // Terminate daemon
    daemon_terminate();

	return 0;
}
