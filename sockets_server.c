#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// /*
//  * SERVER: Socket() -> Bind() -> listen() -> accept() -> read() -> write() -> close()
//  *											^	          ^           v        v ^
//  *											^             ^           v        v ^
//  * CLIENT: Socket() ->---------------------> connection() -> write() -> read() -> close()
//  */

// /*
//  * - sa_family: Address family (e.g., AF_INET for IPv4, AF_INET6 for IPv6)
//  * - sa_data[14]: Raw data for address information (varies by address family)
//  * - return: 0 for success
//  * - return: -1 for failure
//  */

// struct sockaddr
// {
//     sa_family_t sa_family;
//     char sa_data[14];
// };

// /*
//  *	bind function is used to associate a specific socket (identified by file descriptor sockfd) with a specific address and port number.
//  *  The details of the address and port are specified using the 'struct sockaddr *addr' and the length of the address structure 'addrlen'.
//  */

// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// /*
//  *	- listen: listens for connection on the socket and marks the socket (sockfd) as used for incoming connections
//  * 	- backlog: number of connections a system can handle at a single time
//  */

// int listen(int sockfd, int backlog);

// /*
//  *	- sockfd: the file descriptor of socket (from accept())
//  *	- buffer: contains the string we are going to be passing as a message
//  *	- buffer_size: the size of the string (message) being passed
//  */
// int read(newsockfd, buffer, buffer_size);

// /*
//  *	- newsockfd: the file descriptor of socket (from accept())
//  *	- buffer: contains the string we are going to be writing
//  *	- buffer_size: the size of the string (message) to write
//  */

// int write(newsockfd, buffer, buffer_size);

// /*
//  *	- newsockfd: an integer variable that will hold the file descriptor of the newly accepted socket. After a client connection is accepted, 'newsockfd' can be
//  *    used to communicate with the client. Will represent the established connection with the client upon successfull execution.
//  *	- sockfd: socket that server uses to listen for incoming connections
//  *	- (struct socksaddr *) &addr: pointer to struct sockaddr....The 'accept' function will fill this structure with the address information of the client that's connecting to the server
//  */

// int newsockfd = accept(sockfd, (struct sockaddr *) &addr, &addrlen);

/*
 * The 'socket' function is used to create a new socket for network communication.
 *
 * Parameters:
 * - 'domain': Specifies the communication domain, which determines the address format.
 *   Common values include AF_INET (IPv4) and AF_INET6 (IPv6).
 *
 * - 'type': Specifies the socket type, which defines the communication semantics.
 *   Common types include SOCK_STREAM (for TCP) and SOCK_DGRAM (for UDP).
 *
 * - 'protocol': Specifies the protocol to be used with the socket. It often matches the
 *   type but can be set to 0 for the default protocol associated with the chosen type.
 *
 * Returns:
 * - If successful, 'socket' returns a new socket file descriptor that can be used for
 *   sending and receiving data. The file descriptor is non-negative.
 *
 * - On failure, 'socket' returns -1, indicating an error occurred. You can use 'errno'
 *   to obtain more information about the error.
 *
 * Example:
 * - To create a new IPv4 TCP socket, use 'socket(AF_INET, SOCK_STREAM, 0)'.
 * - To create a new IPv6 UDP socket, use 'socket(AF_INET6, SOCK_DGRAM, 0)'.
 */

/*
 * Error handler function
 */

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage Error: %s port_number\nTerminating\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr; // server struct
    struct sockaddr_in cli_addr;  // client struct

    socklen_t clilen;                         // socklen_t: 32bit data type ---- clilen: the size of the client struct
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket file descriptor

    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    memset(&serv_addr, 0, sizeof(serv_addr)); // ensure struct is 0'ed
    portno = atoi(argv[1]);                   // convert string argument port number to integer

    serv_addr.sin_family = AF_INET;         // Declare IPV type: IPV4
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Declare this socket will be bound to all available network interfaces.
    serv_addr.sin_port = htons(portno);     // Convert portno from host byte order to network byte order and assign it to serv_addr.sin_port.

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0) // bind expects a pointer to struct sockaddr therefore we will cast the pointer from 'struct sockaddr_in *' to 'struct sockaddr *'.
    {
        error("Binding Failed\n");
    }

    listen(sockfd, 5);         // Listen on the socket (sockfd) for incoming connections. Allow up to 5 pending connections in the queue.
    clilen = sizeof(cli_addr); // Set 'clilen' to the size of the client address structure, to be used later in 'accept()'.

    /*
     *  Wait for a client to connect and accept the connection. Upon successful connection, store the new socket's file descriptor in 'newsockfd'.
     *  The 'sockfd' is the original listening socket.
     *  'cli_addr' will be filled with the client's address information.
     *  '&clilen' specifies the length of the client address structure.
     */

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) // If accept returns a value less than 0, an error has occurred
    {
        error("Error on accept");
    }

    while (1)
    {

        memset(buffer, 0, sizeof(buffer)); // Clear buffer

        n = read(newsockfd, buffer, sizeof(buffer));

        if (n < 0)
        {
            error("Error on read");
        }
        printf("Client : %s\n", buffer);   // Print clients message
        memset(buffer, 0, sizeof(buffer)); // Clear buffer for next message.

        fgets(buffer, sizeof(buffer), stdin); // Get next message from standard input
        n = write(newsockfd, buffer, sizeof(buffer));
        if (n < 0)
        {
            error("Error on write");
        }

        int i = strncmp("exitme", buffer, 6);

        if (i == 0)
        {
            break;
        }
    }

    // Close our file descriptors
    close(newsockfd);
    close(sockfd);
    // End it
    return 0;
}