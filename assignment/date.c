#include <stdio.h>
#include <time.h>

char* getDate(char * buffer)
{
	//Creating the time and date for the log files
	time_t datatime;
	struct tm * gettime;
	time (&datatime);
	gettime = localtime(&datatime);
	strftime(buffer, 80, "%d_%m_%Y_%H_%M_%S",gettime);
	printf("\nTime:%s",buffer);
	return buffer;
}
