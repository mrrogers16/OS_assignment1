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

	if (argc < 3)
	{
		fprintf(stderr, "USAGE: %s hostname portnumber\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);					  // convert argv portno to int portno
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // create new IPV4 socket

	if (sockfd < 0)
	{
		error("ERROR: opening socket");
	}

	server = gethostbyname(argv[1]); // retrieve host name with server.h gethostbyname()
	if (server == NULL)
	{
		fprintf(stderr, "ERROR: No such host");
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); // Ensure our serv_addr struct is empty

	serv_addr.sin_family = AF_INET;
	/*
	 *	NOTE : server->h_addr is old convention. Use server->h_addr_list[i] now for access to an array of addresses
	 *  Copy the server's IP address from 'serv_addr.sin_addr.s_addr' (in binary form)
	 *  to the 'server->h_addr' field (destination) of the 'hostent' structure.
	 *	server->h_length' specifies the length of the address (typically 4 bytes for IPv4).
	 */
	memcpy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Connection failed");
	}
	while (1)
	{
		memset(buffer, 0, MAXBUFF);
		fgets(buffer, MAXBUFF, stdin);
		n = write(sockfd, buffer, strlen(buffer));

		if (n < 0)
		{
			error("ERROR: on write");
		}

		memset(buffer, 0, MAXBUFF);
		n = read(sockfd, buffer, MAXBUFF);
		if (n < 0)
		{
			error("ERROR: on read");
		}

		printf("Server : %s", buffer);

		int i = strncmp("exitme", buffer, 6);

		if (i == 0)
		{
			break;
		}
	}

	close(sockfd);

	return 0;
}