#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[])
{
    WSAData wsaData;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }


    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET)
    {
        fprintf("ERROR: Failed to create socket: %d \n", WSAGetLastError());
        return 1;
    }

    struct sockaddr_in server;
    server.sin_addr = inet_addr("104.22.55.228");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    WSACleanup();
    return 0;
    
}