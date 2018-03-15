#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <syslog.h>
#include <setjmp.h>
#include "logs.h"

void changepermissions(char mode[])
{
	int x;
	struct stat st;

	//Path of directory we want to change
	char files[100] = "home/jordan/Documents/SystemSoftware/assignment/var/www/";
	stat(files, &st);
	x = strtol(mode,0,8);

	//Not aloud change the permissions
	if(chmod(files, x) < 0)
	{
		loging("Can not change the permission of the folder");
	}

	//Aloud to change the permissions
	else
	{
		loging("The permissions of the folder have changed");
	}
}
