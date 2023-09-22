#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXBUFF 255

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[MAXBUFF];

	if(argc < 3)
	{
		fprintf(stderr, "USAGE: %s hostname portnumber\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0)
	{
		error("ERROR: opening socket");
	}

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "ERROR: No such host");

	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	//memcopy


	return 0;
}