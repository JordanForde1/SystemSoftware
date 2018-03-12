#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define BUFFER_SIZE 1024

int main()
{
	mqd_t mq;
	char buffer[BUFFER_SIZE];
	mq = mq_open("/assignment_queue", O_WRONLY);
	printf("Send message:\n");

	do
	{

		printf(">>");
		fflush(stdout);
		memset(buffer,0,BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);
		printf("%s\n", buffer);
		mq_send(mq, buffer, BUFFER_SIZE, 0);
	}

	while(strncmp(buffer, "exit", strlen("exit")));
	mq_close(mq);

	return 0;
}
