#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

void response(int SID){

	char server_message[500];

	if(recv(SID, server_message, 500, 0) < 0)
	{
		printf("IO error\n");
	}

	if(strcmp(server_message, "Acess granted") == 0)
	{
		printf("User has been authenticated. Connected to server");
	}

	else if(strcmp(server_message, "START_TRANSFER") == 0)
	{
		printf("Starting transfer");
	}

	else if(strcmp(server_message, "TRANSFER_COMPLETE") == 0)
	{
		printf("Transfer complete");	
	}

	else
	{
		printf(server_message);
	}
}

void folderpaths (char *message, char *file_path)
{
	char *files_directory = "/home/jordan/Documents/SystemSoftware/assignment2/development/";
	char folder[500];
	char files[500];

	printf("Please enter file you wish to send: \n");
	scanf("%d", &files);

	strcpy(file_path, files_directory);
	strcat(file_path, files);

	if(access(file_path, 0) != 0)
	{
		printf("File does not exist");
		exit(1);
	}

	printf("\n Please enter where you would like to send the file: \nMarketing \nSales \nPromotions \nOffers ");
	scanf("%s", folder);

	strcpy(message, files);
	strcat(message, " ");
	strcat(message, folder);
}

void authentication (char *message)
{
	char username[500];
	char password[500];
	
	printf("Please login!\n");
	printf("Enter username: ");
	scanf("%s", username);
	printf("Enter password: ");
	scanf("%s", password);

	strcpy(message, username);
	strcat(message, " ");
	strcat(message, password);
}

void transfer (int SID, char *file_path)
{
	char file_buffer[512];
	int block_size = 0;
	int i = 0;

	FILE *file_open;
	file_open = fopen(file_path, "r");

	bzero(file_buffer, 512);

	while((block_size = fread(file_buffer, sizeof(char), 512, file_open)) > 0)
	{
		
		printf("Data sent %d = %d bytes\n", i, block_size);

		if(send(SID, file_buffer, block_size, 0) < 0)
		{
			exit(1);
		}

		bzero(file_buffer, 512);
		i++;
	}
}

int main(int argc, char *argv[])
{

	int SID;
	struct sockaddr_in server;
	char server_message[500];
	char destination_folder[500];	
	char file_folder[500];
	char file_path[500];
	char authentication_login[500];

	SID = socket(AF_INET, SOCK_STREAM, 0);

	if(SID == -1)
	{
		printf("error creating socket\n");
	}

	else
	{
		printf("socket created\n\n");
	}

	server.sin_port = htons(8081);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;

	if(connect(SID, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Connect failed\n");
		return 1;
	}
	
	authentication(authentication_login);

	if(send(SID, authentication_login, strlen(authentication_login),0) < 0)
	{
		printf("Files did not send");
		return 1;
	}

	memset(server_message, 0, 500);
	response(SID);

	folderpaths(file_folder, file_path);
	
	if(send(SID, file_folder, strlen(file_folder), 0) < 0)
	{
		printf("Sending failed\n");
		return 1;
	}

	memset(server_message, 0, 500);

	if(recv(SID, server_message, 500, 0) < 0)
	{
		printf("IO error\n");
	}

	response(SID);
	transfer(SID, file_path);
	response(SID);
	close(SID);
	return 0;
}
