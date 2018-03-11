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

	char *getFiles = "/home/jordan/Documents/SystemSoftware/assignment/var/www/html/intranet/";
	char *putFiles ="/home/jordan/Documents/SystemSoftware/assignment/var/www/html/live/";
	char *sync = "rsync -r /home/jordan/Documents/SystemSoftware/assignment/var/www/html/intranet/ /home/jordan/Documents/SystemSoftware/assignment/var/www/html/live/";

	FILE *fp;
	FILE *output;
	int over;
	char readbuffer[1024];

	fp = popen(sync, "r");
	output =  fopen("/home/jordan/Documents/SystemSoftware/assignment/filesLog.txt", "a+");

	while(fgets(readbuffer, 1024, fp) != NULL)
	{

		fprintf(output, "%s", readbuffer);
	}

	loginformation("Files have been succsfully synced with the live web page");
	over = pclose(fp);
}
