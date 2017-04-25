/*
 *  Author: Sandeeep Kumar Mittal 
 *  Special Thanks to Prabodh Agarwal
 *  Description: Server side file to guess a number over TCP/IP sockets.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> 

int main()
{

    // Important declaration
    int serverSocket = 0, newSocket = 0;
    char *recvBuffer, *buffer;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddress;
    socklen_t addr_size;
    int randomNumber = 0, recvBytes = 0, recvNumber = 0;

    recvBuffer = (char *)malloc(sizeof(char)*10);
    buffer = (char *)malloc(sizeof(char)*10);

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
    memset ( serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero) );

    memset(recvBuffer, 0, 10);
    memset(buffer, 0, 10);

    if ( bind ( serverSocket, ( struct sockaddr *) &serverAddr, sizeof ( serverAddr ) )  < 0)	//binding
        printf("[ERROR] Socket binding Failed. \n");

    if ( listen ( serverSocket, 5 ) ==0 )//listening
        printf ( "Server Scocket Initiated. Listening to its clients : \n" );
    else
        printf("[ERROR] More than limit.\n");

    newSocket = accept ( serverSocket, (struct sockaddr *) &clientAddress, &addr_size);//accept call
    
    while ( 1 )
    {
        addr_size = sizeof ( clientAddress );
        if ( newSocket < 0)
            printf("[ERROR] Failed to accept the input.\n");

        memset(recvBuffer, 0, 10);
        if ( ( recvBytes = recv ( newSocket, recvBuffer, 9, 0) == -1))
            printf("[ERROR] No input received from client.\n");

        printf("Handling client:  %s \n", inet_ntoa(clientAddress.sin_addr));
        printf("Received Packet contains \"%s\"\n\n", recvBuffer);

        if(strcmp(recvBuffer, "DONE") == 0)
        {
            printf("DONE\n\n");
            break;
        }
        if ( recvNumber == randomNumber )
        {
            strcpy( buffer, "1\0");
        } else {
            strcpy( buffer, "2\0");
        }
		strcpy( buffer, "received\0");
        send  ( newSocket, buffer, 10, 0 );
        sleep ( 1 );
    }
    close ( newSocket );
    close ( serverSocket );
    return 0;
}
