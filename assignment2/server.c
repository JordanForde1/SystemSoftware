#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

//Funnction to identify the user
int login(char *username, char *password)
{
	//veriables to campare
	char uname[500];
	char pass[500];
	int correct = 0;

	//Open users file for reading to compare username and password
	FILE *login_file;
	login_file = fopen("users.txt", "r");

	//Do if there is no error
	while(fscanf(login_file, "%s %s", uname, pass) != EOF)
	{
		//Make sure both are correct
		if(strcmp(username, uname) == 0 && strcmp(password, pass) == 0)
		{
			correct = 1;
			return 1;
		}
	}
	
	//Returning 1 if it is correct
	if(correct == 1)
	{
		fclose(login_file);
		return 1;
	}
	
	//Returning 0 if its not
	else
	{
		fclose(login_file);
		return 0;
	}
}

//Function for sever to reveive data and do things with it
void *clientAuth(void *cs)
{
	//Veribales needed
	char file_buffer[512];
	char file_des[512];
	int client_socket = *(int *) cs;
	char username[500];
	char password[500];
	char file[500];
	char folder[500];
	char *server_directory = "/home/jordan/Documents/SystemSoftware/assignment2/intranet/";
	char output_location[500];
	int block_size = 0;

	//Time veriables for time stamos
	time_t now;
	struct tm timeinfo;

	//Clear
	bzero(file_buffer, 512);
	bzero(file_des, 512);

	//Recieve user data from the client
	recv(client_socket, file_des, 512, 0);
	printf("Received message: %s\n", file_des);
	sscanf(file_des, "%s %s", username, password);
	printf("%s\n%s\n", username, password);

	//Give access to the user and proceed
	if(login(username, password) == 1)
	{
		printf("User has been authenticated\n");
		write(client_socket, "Acess granted\n", strlen("User has been authenticated\n") + 1);

	}

	//Denie access to the user and stop the server
	else
	{
		printf("Not a user on the system\n");
		write(client_socket, "Denied Access\n", strlen("Not a user on the system\n") + 1);
		exit(1);
	}
	
	bzero(file_des, 512);	
	
	//Recieveing location data from the user
	recv(client_socket, file_des, 512, 0);
	sscanf(file_des, "%s %s", file, folder);
	printf("%s\n%s\n", file, folder);

	//Making usre folder selected is on the system
	if(strcmp(folder, "marketing") != 0 && strcmp(folder, "offers") != 0 && strcmp(folder, "sales") != 0 && strcmp(folder, "promotions") != 0 )
	{
		write(client_socket, "Not a valid folder option", strlen("Not a valid folder option") + 1);
		exit(1);
	}

	bzero(file_des, 512);

	//Put fill file address in
	strcpy(output_location, server_directory);
	strcat(output_location, folder);
	strcat(output_location, "/");
	strcat(output_location, file);

	//Open a file for writing
	FILE *output;
	output = fopen(output_location, "w");

	while((block_size = recv(client_socket, file_buffer, 512, 0)) > 0)
	{
	
		printf("\nData received: %d\n", block_size);
		
		//Log file used to keep track of who has done what
		FILE *log = fopen("/home/jordan/Documents/SystemSoftware/assignment2/logs.txt","a");

		//Time veriables
		char logging[100];
		time(&now);
		timeinfo = *localtime(&now);

		//Writing to log file nessary details
		strcpy(logging, "Username: ");
		strcat(logging, username);
		strcat(logging, "\nFile location: ");
		strcat(logging, output_location);
		strcat(logging, "\nTime: ");
		strcat(logging, ctime(&now));

		//The acctual write call
		fprintf(log, "%s\n", logging);

		fclose(log);

		int write_file = fwrite(file_buffer, sizeof(char), block_size, output);
		bzero(file_des, 512);
		memset(file_buffer, 0, sizeof(file_buffer));

		if(write_file == 0 || write_file != 512)
		{
			break;
		}

	}

	//close
	fclose(output);
	
	//Say transpher was complete and client exits
	write(client_socket, "TRANSFER_COMPLETE", strlen("TRANSFER_COMPLETE") + 1);
}

int main(int argc, char *argv[])
{
	//socket descripter
	int s;

	//client socket
	int cs;

	//struct size
	int consize;

	//size of sockaddr_in for client connection
	int READSIZE;

	struct sockaddr_in server, client;
	char message[500];

	char file_buffer[512];

	//Creating socket
	s = socket(AF_INET, SOCK_STREAM, 0);

	if ( s== -1 )
	{
		printf("Cloud not create socket\n");
	}
	
	else
	{	
		printf("Socket created successfully\n");
	}

	//port for communication
	server.sin_port = htons(8081);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	//bind
	if(bind(s,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Bind issue\n");
		return 1;
	}

	else
	{
		printf("Bind complete\n");

	}

	//listen for a connection
	listen(s,3);

	//accept and incoming connection
	printf("Waiting incoming connection form client>>\n");
	consize = sizeof(struct sockaddr_in);

	while(cs = accept(s, (struct sockaddr *) &client, (socklen_t*) &consize))
	{

		//Cant connect
		if(cs < 0)
		{
			perror("Cant establish connection\n");
			return 1;
		}

		//Can connect
		else
		{
			printf("Connection form client accepted\n");

			//Declare pthread
			pthread_t thread;

			// Allocates memory for client socket
			int *client_socket = malloc(200);
			*client_socket = cs;
		
			//Using pthread
			if(pthread_create(&thread, NULL, clientAuth, (void *) client_socket) < 0)
			{
				perror("Failed to create thread for client");
			}
		}
	}

	return 0;
}
