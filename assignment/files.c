#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include "date.h"
#include "logs.h"

void files()
{
	char *sync = "rsync -r /home/jordan/Documents/SystemSoftware/assignment/var/www/html/intranet/ /home/jordan/Documents/SystemSoftware/assignment/var/www/html/live/";

	FILE *fp;
	FILE *output;
	char readbuffer[1024];

	fp = popen(sync, "r");
	output =  fopen("/home/jordan/Documents/SystemSoftware/assignment/syncinglogfile.txt", "a+");

	while(fgets(readbuffer, 1024, fp) != NULL)
	{
		fprintf(output, "%s", readbuffer);
	}

	loging("Files have been succsfully synced with the live web page");
	pclose(fp);
}
