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
//int is_client_disconnected = 0;
//int is_listening_for_connections = 0;


void SignalHandler(int sig);
void TalkingToClient();
//void TalkingToClientOrig();

int main(int argc, char const* argv[])
{
	//int recv_response;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	//char recv_message;
	pid_t pid;
	//char char_from_file;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);

	// Creating socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
		syslog(LOG_ERR,"socket creation failed");
		//printf("Socket creation failed\n");
		return -1;
	}

	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    {
		syslog(LOG_ERR,"failed setting socket options");
		//printf("Failed setting socket options\n");
		return -1;
	}

	if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0) 
    {
		syslog(LOG_ERR,"bind to socket failed");
		//printf("Bind to socket failed\n");
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

	// //open file
	fp = fopen("/var/tmp/aesdsocketdata","a+");

	//loop until SIGINT or SIGTERM
	while(1)
	{
		//printf("Listening for connections.....\n");

		if (listen(server_fd, 5) < 0) 
		{
			syslog(LOG_ERR,"Listening for connections failed");
			//printf("Listening for connections failed\n");
			return -1;
		}

		if ((accepted_connection = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) 
		{
			syslog(LOG_ERR,"Accepting connections failed");
			//printf("Accepting connections failed\n");
			return -1;
		}

		inet_ntop(AF_INET, &(address.sin_addr), connected_ip, INET_ADDRSTRLEN);

		syslog(LOG_USER,"Accepted connection from %s", connected_ip);
		//printf("Accepted connection from %s\n", connected_ip);

		TalkingToClient();

		// //client disconnected
		// close(accepted_connection);
		// shutdown(accepted_connection, SHUT_RDWR);
	    syslog(LOG_USER,"Closed connection from %s", connected_ip);
		//printf("Closed connection from %s\n", connected_ip);

		///signal(SIGTERM, SignalHandler);

		// fclose(fp);
		// syslog(LOG_USER,"removing file...");
		// printf("removing file...\n");
		// remove("/var/tmp/aesdsocketdata");
	}

	// closing the listening socket
	//shutdown(server_fd, SHUT_RDWR);
	
	return 0;
}

void TalkingToClient()
{
	int recv_response, buffer_item_count = 1;
	char recv_message = 'x';
	char char_from_file;
	char buffer[4000] = {0};
	//int is_client_disconnected = 0;
	while(recv_message != '\n')
	{
		recv_response = recv(accepted_connection, &recv_message, 1, 0);

		if(recv_response < 0)
		{
			exit(-1);
		}

		if(recv_response == 0)
		{
			signal(SIGTERM, SignalHandler);
		}

		fprintf(fp, "%c", recv_message);
		//printf("%c", recv_message);
		
		// int cnt = 1;
		// if(recv_message == '\n')
		// {
		// 	while(cnt < buffer_item_count)
		// 	{
		// 		fprintf(fp, "%c", buffer[cnt]);
		// 		cnt++;
		// 	}
		// }
		// else
		// {
		// 	// fprintf(fp, "%c", recv_message);
		// 	// printf("%c", recv_message);
		// 	buffer[buffer_item_count] = recv_message;
		// 	buffer_item_count++;
		// }
	}

	//read from file and send back all data
	rewind(fp);
	while (!feof(fp)) 
	{
		char_from_file = fgetc(fp);
		if(feof(fp)) break;
		send(accepted_connection, &char_from_file, 1, 0);
	}
}

// void TalkingToClientOrig()
// {
// 	int recv_response;
// 	char recv_message;
// 	char char_from_file;
// 	is_client_disconnected = 0;
// 	recv_response = 1;
// 	while(is_client_disconnected == 0)
// 	{
// 		recv_message = 'x';
// 		while(recv_message != '\n')
// 		{
// 			recv_response = recv(accepted_connection, &recv_message, 1, 0);

// 			if(recv_response < 0)
// 			{
// 				//is_listening_for_connections = 1;
// 				return -1;
// 			}
// 			else if(recv_response != 0)
// 			{
// 				fprintf(fp, "%c", recv_message);
// 				printf("%c", recv_message);
// 			}
// 			else
// 			{
// 				// syslog(LOG_USER,"Closed connection from %s", connected_ip);
// 				// printf("Closed connection from %s\n", connected_ip);
// 				is_client_disconnected = 1;
// 				break;
// 			}
// 		}

// 		if(is_client_disconnected == 0)
// 		{
// 			//read from file and send back all data
// 			rewind(fp);
// 			while (!feof(fp)) 
// 			{
// 				char_from_file = fgetc(fp);
// 				if(feof(fp)) break;
// 				send(accepted_connection, &char_from_file, 1, 0);
// 			}
// 		}
// 	}
// }

void SignalHandler(int sig)
{
	syslog(LOG_USER,"Caught signal, exiting");
	//printf("\nCaught signal, exiting\n");

	fclose(fp);
	syslog(LOG_USER,"removing file...");
	//printf("removing file...\n");
	remove("/var/tmp/aesdsocketdata");

    // closing the connected socket
	close(accepted_connection);
	close(server_fd);
	// syslog(LOG_USER,"Closed connection from %s", connected_ip);
	// printf("Closed connection from %s\n", connected_ip);

    shutdown(accepted_connection, SHUT_RDWR);
	shutdown(server_fd, SHUT_RDWR);
	

	//is_client_disconnected = 1;
	//is_listening_for_connections = 1;
}
