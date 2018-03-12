#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

void loginformation (char* logs)
{
	printf("\nin logs");
	openlog("assignment", LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "%s", logs);
	closelog();
	printf("\nclose log");
}

void logerrors (char* logs)
{
	openlog("assignment", LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "%s", logs);
	closelog();
}

void logwarnings (char* logs)
{
	openlog("assignment", LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "%s", logs);
	closelog();
}


