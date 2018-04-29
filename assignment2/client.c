#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

//Taking response from the server to keep track of whats happening
void response(int SID){

	char server_message[500];

	if(recv(SID, server_message, 500, 0) < 0)
	{
		printf("Cannot preform input or output functionality\n");
	}

	if(strcmp(server_message, "Acess granted") == 0)
	{
		printf("User has been authenticated. Connected to server\n");
	}

	else if(strcmp(server_message, "START_TRANSFER") == 0)
	{
		printf("\nStarting transfer\n");
	}

	else if(strcmp(server_message, "TRANSFER_COMPLETE") == 0)
	{
		printf("\nTransfer complete\n");	
	}

	else
	{
		printf(server_message);
	}
}

//Fuction to pick file to send and where to send it to
void folderpaths (char *message, char *file_path)
{
	char *files_directory = "/home/jordan/Documents/SystemSoftware/assignment2/development/";
	char folder[500];
	char files[500];

	//Enter the file
	printf("Please enter file you wish to send: \n");
	scanf("%s", &files);
	
	//Adding file picked to directory
	strcpy(file_path, files_directory);
	strcat(file_path, files);
	
	//File is not there
	if(access(file_path, 0) != 0)
	{
		printf("File does not exist\n");
		exit(1);
	}

	//Entering in the location
	printf("\nPlease enter where you would like to send the file: \nMarketing \nSales \nPromotions \nOffers\n");
	scanf("%s", folder);

	strcpy(message, files);
	strcat(message, " ");
	strcat(message, folder);
}

//Autentication fucntion for the user
void authentication (char *message)
{

	//Username and password
	char username[500];
	char password[500];
	
	//Enter details
	printf("Please login!\n");
	printf("Enter username: ");
	scanf("%s", username);
	printf("Enter password: ");
	scanf("%s", password);

	strcpy(message, username);
	strcat(message, " ");
	strcat(message, password);
}

//Transfer function to send the files
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
		
		printf("\nData sent %d = %d bytes\n", i, block_size);

		if(send(SID, file_buffer, block_size, 0) < 0)
		{
			exit(1);
		}

		bzero(file_buffer, 512);
		i++;
	}
}

//Main function
int main(int argc, char *argv[])
{

	//Veribales needed
	int SID;
	struct sockaddr_in server;
	char server_message[500];
	char destination_folder[500];	
	char file_folder[500];
	char file_path[500];
	char authentication_login[500];

	SID = socket(AF_INET, SOCK_STREAM, 0);

	//Make sure socket is created
	if(SID == -1)
	{
		printf("error creating socket\n");
	}

	else
	{
		printf("Socket created\n\n");
	}

	//Server veribales
	server.sin_port = htons(8081);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;

	//Make sure client connects
	if(connect(SID, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Connect failed\n");
		return 1;
	}
	
	//Autenticate user
	authentication(authentication_login);

	//Send to server if greater then 0
	if(send(SID, authentication_login, strlen(authentication_login),0) < 0)
	{
		printf("Did not send user details");
		return 1;
	}

	//Reset memory
	memset(server_message, 0, 500);
	response(SID);

	//Send where the files are going
	folderpaths(file_folder, file_path);

	//Send to server if greater then 0
	if(send(SID, file_folder, strlen(file_folder), 0) < 0)
	{
		printf("Sending failed\n");
		return 1;
	}

	//Send and response
	transfer(SID, file_path);
	response(SID);
	close(SID);
	return 0;
}
