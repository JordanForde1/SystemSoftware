#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

//creating function to write to syslog for error handling
void loging (char* logs)
{
	openlog("assignment", LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "%s", logs);
	closelog();
}

