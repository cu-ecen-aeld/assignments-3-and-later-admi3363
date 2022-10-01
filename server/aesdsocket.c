#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>


#define PORT 9000

int server_fd;
int accepted_connection;
FILE *fp = NULL;
char connected_ip[INET_ADDRSTRLEN];


void sigintHandler(int sig);

int main(int argc, char const* argv[])
{
	int recv_response;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	//char buffer[2000] = { 0 };
	//size_t current_size = 0;
	char recv_message;
	pid_t pid;
	char char_from_file;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	

	signal(SIGINT, sigintHandler);
	signal(SIGTERM, sigintHandler);


	// Creating socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
		syslog(LOG_ERR,"socket creation failed");
		return -1;
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
		syslog(LOG_ERR,"failed setting socket options");
		return -1;
	}

	if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0) 
    {
		syslog(LOG_ERR,"bind to socket failed");
		return -1;
	}

	//fork after binding to port
	if(argc==2 && argv[1][1] == 'd') 
	{
		pid = fork();

		if (setsid() < 0)
		{
     		return -1;
		}
	
		if (pid > 0)
		{
			return -1;
		}
    }

	//open file
	fp = fopen("/var/tmp/aesdsocketdata","w+");

	//loop until SIGINT or SIGTERM
	while(1)
	{
		if (listen(server_fd, 5) < 0) 
		{
			syslog(LOG_ERR,"listen failed");
			return -1;
		}

		if ((accepted_connection = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) 
		{
			syslog(LOG_ERR,"accept failed");
			return -1;
		}

		inet_ntop(AF_INET, &(address.sin_addr), connected_ip, INET_ADDRSTRLEN);

		syslog(LOG_USER,"Accepted connection from %s", connected_ip);
		printf("Accepted connection from %s\n", connected_ip);

		//recv_message = 'x';
		while(recv_message != '\n')
		{
			recv_response = recv(accepted_connection, &recv_message, 1, 0);

			if(recv_response < 0)
			{
				return -1;
			}
                        else if(recv_response == 0)
                        {
                         	syslog(LOG_USER, "Client disconnected");
				//fflush(accepted_connection);
				break;
                        }

			fprintf(fp, "%c", recv_message);
			printf("%c", recv_message);
		}

		//fprintf(fp, "\n");

		//read from file and send back all data
		rewind(fp);
		while (!feof(fp)) 
		{
			char_from_file = fgetc(fp);
			send(accepted_connection, &char_from_file, 1, 0);
		}

		//send(accepted_connection, "\n", 0, 0);
		//send(accepted_connection, "string", 6, 0);

		//client disconnected
	}
	
	// fclose(fp);
	// syslog(LOG_USER,"removing file...");
	// printf("removing file...\n");
	// remove("/var/tmp/aesdsocketdata");

	// // closing the connected socket
	// close(accepted_connection);
	// syslog(LOG_USER,"Closed connection from %s", connected_ip);
	// printf("Closed connection from %s\n", connected_ip);
	// // closing the listening socket
	// shutdown(server_fd, SHUT_RDWR);
	return 0;
}

void sigintHandler(int sig)
{
	syslog(LOG_USER,"Caught signal, exiting");
	printf("\nCaught signal, exiting\n");

    // closing the connected socket
	close(accepted_connection);
	syslog(LOG_USER,"Closed connection from %s", connected_ip);
	printf("Closed connection from %s\n", connected_ip);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);

	fclose(fp);
	syslog(LOG_USER,"removing file...");
	printf("removing file...\n");
	remove("/var/tmp/aesdsocketdata");
}