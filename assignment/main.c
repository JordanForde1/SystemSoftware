#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>	
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <setjmp.h>
#include <mqueue.h>
#include <signal.h>
#include "backupfiles.h"
#include "files.h"
#include "changepermissions.h"
#include "logs.h"
#include "watchingfiles.h"
#include "date.h"

#define BUFFER_SIZE 1024

int main()
{
	char * watchfile = "auditctl -w /home/jordan/Documents/SystemSoftware/assignment/var/www/html/ -p rwxa";

	FILE *fp;
	FILE *output;
	char readBuffer[1024];

	fp = popen(watchfile, "r");
	output = fopen("/home/jordan/Documents/SystemSoftware/assignment/auditinglogfile.txt", "a+");

	while(fgets(readBuffer, 1024, fp) != NULL)
	{
		fprintf(output, "%s", readBuffer);
	}
	
	pclose(fp);
	pid_t pid = fork();

	//parent
	if(pid > 0)
	{
		loging("Exit parent");
		exit(EXIT_SUCCESS);
	}

	else if (pid == 0)
	{
		if(setsid < 0)
		{
			exit(EXIT_FAILURE);
		}

		umask(0);

		if(chdir("/") < 0)
		{
			exit(EXIT_FAILURE);
		}

		int i;

		for(i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
		{

			close(i);
		}

		mqd_t mq;
		struct mq_attr queue_atrributes;
		char buffer[BUFFER_SIZE + 1];
		int terminate = 0;

		queue_atrributes.mq_flags = 0;
		queue_atrributes.mq_maxmsg = 10;
		queue_atrributes.mq_msgsize = BUFFER_SIZE;
		queue_atrributes.mq_curmsgs = 0;

		mq = mq_open("/assignment_queue", O_CREAT | O_RDONLY, 0644, &queue_atrributes);

		do
		{
			ssize_t bytes_read;
			bytes_read = mq_receive(mq, buffer, BUFFER_SIZE, NULL);
			loging(buffer);
			
			buffer[bytes_read] = '\0';
			
			if(!strncmp(buffer, "exit", strlen("exit")))
			{
				terminate = 1;
				loging("Daemon is finished");
			}
			
			else if(!strncmp(buffer, "backup", strlen("backup")))
			{
				loging("Starting back up now\n");
				changepermissions("1111");
				backupfiles();
				files();
				watchfiles();
				loging("Backup and syncnronise files");
				changepermissions("0777");
			}
		}
		

		while(terminate < 1);
	}
	return 0;
}
