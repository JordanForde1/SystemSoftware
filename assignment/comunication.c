#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define BUFFER_SIZE 1024

int main()
{
	mqd_t mq;
	struct mq_attr queue_atrributes;
	char buffer[BUFFER_SIZE + 1];

	queue_atrributes.mq_flags = 0;
	queue_atrributes.mq_maxmsg = 10;
	queue_atrributes.mq_msgsize = BUFFER_SIZE;
	queue_atrributes.mq_curmsgs = 0;

	if((mq = mq_open("/assignment_queue", O_CREAT | O_WRONLY, 0644, &queue_atrributes)) == -1)
	{
		perror("mq_open");
	}
	
	printf("Send message:\n");

	do
	{
		printf(">>");
		fflush(stdout);
		memset(buffer,0,BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);
		printf("%s\n", buffer);
		if((mq_send(mq, buffer, BUFFER_SIZE, 0)) == -1)
		{
			perror("mq_send");
		}
	}

	while(strncmp(buffer, "exit", strlen("exit")));
	mq_close(mq);

	return 0;
}
