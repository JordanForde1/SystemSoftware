#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include "date.h"
#include "logs.h"

void watchfiles()
{
	char datebuffer[80];
	char *date = getDate(datebuffer);
	char *auditsearch = "ausearch -f /home/jordan/Documents/SystemSoftware/assignment/var/www/html/ > /home/jordan/Documents/SystemSoftware/assignment/logs/";
	char *filetype = ".txt";

	int size = strlen(auditsearch) + strlen(date) + strlen(filetype) + 1;
	char *sizebuffer = (char *)malloc(size);
	
	strcpy(sizebuffer, auditsearch);
	strcat(sizebuffer, date);
	strcat(sizebuffer, filetype);
	
	FILE *fp;
	FILE *output;
	int over;
	char readbuffer[1024];

	fp = popen(sizebuffer, "r");
	output =  fopen("/home/jordan/Documents/SystemSoftware/assignment/backupfilelog.txt", "a+");

	while(fgets(readbuffer, 1024, fp) != NULL)
	{

		fprintf(output, "%s", readbuffer);
	}

	loginformation("Files have been succsfully backed up to the back up folder");
	over = pclose(fp);
}