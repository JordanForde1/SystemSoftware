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
	//To copy the files from the intranet directoy to the live using rsync
	char *sync = "rsync -r /home/jordan/Documents/SystemSoftware/assignment/var/www/html/intranet/ /home/jordan/Documents/SystemSoftware/assignment/var/www/html/live/";

	//File distriptors and output file
	FILE *fp;
	FILE *output;
	char readbuffer[1024];

	//directory to make log file
	fp = popen(sync, "r");
	output = fopen("/home/jordan/Documents/SystemSoftware/assignment/commandlog/fileslog.txt", "a+");

	while(fgets(readbuffer, 1024, fp) != NULL)
	{
		fprintf(output, "%s", readbuffer);
	}

	loging("Files have been succsfully synced with the live web page");
	
	//closing file discriptors
	//pclose(fp);
}
