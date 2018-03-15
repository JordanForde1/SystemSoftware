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
	//Command to watch a directory for any changes put in place by a user
	char * watchfile = "auditctl -w /home/jordan/Documents/SystemSoftware/assignment/var/www/html/ -p rwxa";

	//File discriptors and output file
	FILE *fp;
	FILE *output;

	//read buffer
	char readBuffer[1024];

	
	//Opening file discriptors
	fp = popen(watchfile, "r");

	//Directory for output file
	output = fopen("/home/jordan/Documents/SystemSoftware/assignment/commandlog/mainlog.txt", "a+");

	while(fgets(readBuffer, 1024, fp) != NULL)
	{
		fprintf(output, "%s", readBuffer);
	}
	
	//close file discriptors
	pclose(fp);

	//Calling the fork command to create a child process
	pid_t pid = fork();

	// if pid > 0 this is the parent process
	if(pid > 0)
	{
		//Want to exit this processs
		loging("Exit parent");
		exit(EXIT_SUCCESS);
	}

	//This is the child process
	else if (pid == 0)
	{
		//This command runs the process in a new session
		if(setsid < 0)
		{
			exit(EXIT_FAILURE);
		}
		//umask allows the daemon to read/write files with the correct permissions
		umask(0);

		//Change the current directory to root
		if(chdir("/") < 0)
		{
			exit(EXIT_FAILURE);
		}

		//Closing all file discriptors
		int i;

		for(i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
		{

			close(i);
		}

		//veribales for queue and termination
		mqd_t mq;
		struct mq_attr queue_atrributes;
		char buffer[BUFFER_SIZE + 1];
		int terminate = 0;

		//queue attributes
		queue_atrributes.mq_flags = 0;
		queue_atrributes.mq_maxmsg = 10;
		queue_atrributes.mq_msgsize = BUFFER_SIZE;
		queue_atrributes.mq_curmsgs = 0;
		
		//Opening the message queue
		mq = mq_open("/assignment_queue", O_CREAT | O_RDONLY, 0644, &queue_atrributes);
		
		//infinate loop
		do
		{
			//having the queue receive data
			ssize_t bytes_read;
			bytes_read = mq_receive(mq, buffer, BUFFER_SIZE, NULL);
			
			buffer[bytes_read] = '\0';
			
			//if its 'exit, end the daemon
			if(!strncmp(buffer, "exit", strlen("exit")))
			{
				terminate = 1;
				loging("Daemon is finished");
			}
			
			//if back up preform the back upp
			else if(!strncmp(buffer, "backup", strlen("backup")))
			{
				loging("Starting back up now\n");

				//Change permission so no one can edit 
				changepermissions("1111");

				//Back up files
				backupfiles();

				//Copy files to live
				files();

				//Creating logs
				watchfiles();
				loging("Backup and syncnronise files");

				//Change permissions back
				changepermissions("0777");
			}
		}
		
		//end loop
		while(terminate < 1);
	}
	return 0;
}
