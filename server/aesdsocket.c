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


void sigintHandler(int sig);

int main(int argc, char const* argv[])
{
	int read_size;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[2000] = { 0 };
	size_t current_size = 0;
	char recv_message;
	pid_t pid;

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

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

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

	fp = fopen("/var/tmp/aesdsocketdata","w+");

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

		syslog(LOG_USER,"Accepted a connection");

		// int recv_response
		// do
		// {
		// 	recv_response = recv(accepted_connection, &recv_message, 1, 0);
		// 	if(recv_response < 0) 
		// 	{
		// 		return -1;
		// 	}

		// 	fprintf(fp, "%c", recv_message);
		// } while (recv_message != '\n');



		int recv_response;
		while(recv_message != '\n')
		{
			recv_response = recv(accepted_connection, &recv_message, 1, 0);
			fprintf(fp, "%c", recv_message);
			//write(fp, recv_message, strlen(recv_message));
			//buffer[current_size++] = recv_message;
		}

		int i = 0;
		while(i < current_size)
		{
			//recv_response = recv(accepted_connection, &recv_message, 1, MSG_WAITALL);
			//write(accepted_connection, buffer[current_size], 1);
			current_size--;
		}

		int justwanttoseeifigethere;
		printf("I got where i was looking");

		//client disconnected
	}
	
	fclose(fp);
	syslog(LOG_USER,"removing file...");
	remove("/var/tmp/aesdsocketdata");

	// closing the connected socket
	close(accepted_connection);
	syslog(LOG_USER,"Closed client connection");
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

void sigintHandler(int sig)
{
	syslog(LOG_USER,"Caught signal, exiting");

    // closing the connected socket
	close(accepted_connection);
	syslog(LOG_USER,"Closed client connection");
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);

	fclose(fp);
	syslog(LOG_USER,"removing file...");
	remove("/var/tmp/aesdsocketdata");
}
