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
	//Command to copy files from the directory	
	char *copy = "cp -r ";

	//Where we are coping the files from
	char *getfiles = "/home/jordan/Documents/SystemSoftware/assignment/var/www/html/intranet/ ";

	//Where we want to copy the files to
	char *putfiles = "/home/jordan/Documents/SystemSoftware/assignment/var/www/html/backup/";

	//Buffer for reading in the date
	char datebuffer[80];
	char *date = getDate(datebuffer);

	//Getting the size of the files 
	int putfilessize = strlen(putfiles) + strlen(date) + 1;
	char *putfiledate = (char *)malloc(putfilessize);

	//Copying and concatinating
	strcpy(putfiledate, putfiles);
	strcat(putfiledate, date);
	
	//Where we are putting thr files make sure its the right size
	int copysize =  strlen(copy) + strlen(getfiles) + strlen(putfiledate) + 1;
	char *copycommand = (char *)malloc(copysize);
	
	//Copying and concatinating
	strcpy(copycommand, copy);
	strcat(copycommand, getfiles);
	strcat(copycommand, putfiledate);

	//File discriptors
	FILE *fp;

	//Output file
	FILE *output;

	//Buffer reader
	char readbuffer[1024];

	fp = popen(copycommand, "r");
	output = fopen("/home/jordan/Documents/SystemSoftware/assignment/commandlog/backupfileslog.txt", "a+");

	while(fgets(readbuffer, 1024, fp) != NULL)
	{
		fprintf(output, "%s", readbuffer);
	}

	loging("Files have been succsfully backed up to the back up folder");

	//Closing the file discriptor
	pclose(fp);
}
