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

	// Set the time to back up at mid-night
	then.tm_hour = 14;
	then.tm_min = 40;
	then.tm_sec = 0;

	// Loop to print each second
	while(1)
	{
		time(&now);
		seconds = difftime(now, mktime(&then));

		if(seconds == 0)
		{
			loging("Timer has reached zero.\n");
			loging("Starting back up now\n");
			changepermissions("1111");
			backupfiles();
			files();
			watchfiles();
			loging("Back up and syncnronise files");
			changepermissions("0777");
		}
	}
}
