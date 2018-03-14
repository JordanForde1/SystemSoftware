#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include "date.h"
#include "logs.h"

void backupfiles()
{
	char *copy = "cp -r";
	char *getfiles = "/home/jordan/Documents/SystemSoftware/assignment/var/www/html/intranet/";
	char *putfiles = "/home/jordan/Documents/SystemSoftware/assignment/var/www/html/backup/";
	char datebuffer[80];
	char *date = getDate(datebuffer);
	
	printf("\nDate: %s", date);
	int putfilessize = strlen(putfiles) + strlen(date) + 1;
	char *putfiledate = (char *)malloc(putfilessize);

	strcpy(putfiledate, putfiles);
	strcat(putfiledate, date);

	int copysize =  strlen(copy) + strlen(getfiles) + strlen(putfiledate) + 1;
	char *copycommand = (char *)malloc(copysize);

	strcpy(copycommand, copy);
	strcat(copycommand, " ");
	strcat(copycommand, getfiles);
	strcat(copycommand, putfiledate);

	FILE *fp;
	FILE *output;
	char readbuffer[1024];

	fp = popen(copycommand, "r");
	output =  fopen("/home/jordan/Documents/SystemSoftware/assignment/backupfilelog.txt", "a+");

	while(fgets(readbuffer, 1024, fp) != NULL)
	{

		fprintf(output, "%s", readbuffer);
	}

	loginformation("Files have been succsfully backed up to the back up folder");
}
