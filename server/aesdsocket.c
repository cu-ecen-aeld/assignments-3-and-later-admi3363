#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>


#define PORT 9000

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, read_size;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char read_message[2000];

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

	if (listen(server_fd, 3) < 0) 
    {
		syslog(LOG_ERR,"listen failed");
		return -1;
	}

	if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) 
    {
		syslog(LOG_ERR,"accept failed");
		return -1;
	}

	FILE *fp = fopen("/var/tmp/aesdsocketdata","w");

	while( 1==1 )
	{
		if((read_size = recv(new_socket , read_message , 2000 , 0)) > 0 )
		{
			//fwrite(string, sizeof char, strlen(string), fp);
			fprintf(fp, "%s", read_message);
		}
	}

	syslog(LOG_USER,"Caught signal, exiting");
	
	fclose(fp);
	syslog(LOG_USER,"removing file...");
	remove("/var/tmp/aesdsocketdata");

	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}