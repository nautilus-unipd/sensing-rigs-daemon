#include "logger.h"
#include "messages.h"
#include "daemon_functions.h"

int main(void)
{
    // Initialise daemon
	daemon_create();

    // Daemon's main loop
	while(1)
	{
        append_log(INFO, "Message inside loop");
        sleep(5);
        append_log(INFO, " inside loop");
        break;
	}

    append_log(INFO, "Terminated daemon main loop");
    // Terminate daemon
    daemon_terminate();

	return 0;
}
