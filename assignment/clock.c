#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
	// Create time variables
	time_t now;
	struct tm then;
	double seconds;

	// Set the current time
	time(&now);
	then = *localtime(&now);

	// Set the time to count down to
	then.tm_hour = 14;
	then.tm_min = 40;
	then.tm_sec = 0;

	// Loop to print each second
	while(1)
	{
		time(&now);
		seconds = difftime(now, mktime(&then));
		printf("Seconds: %f\n", seconds);
		sleep(1);

		if(seconds == 0)
		{
			printf("Timer has reached zero.\n");
		}
	}
}
