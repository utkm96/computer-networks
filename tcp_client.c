/*
 *  Author: Sandeeep Kumar Mittal 
 *  Special Thanks to Prabodh Agarwal
 *  Description: Client side file to guess a number over TCP/IP sockets.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int clientSocket;
    char *buffer;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // User Input
    char *userInput;

    userInput = (char *)malloc(sizeof(char)*10);
    buffer = (char *)malloc(sizeof(char)*10);

    /*  Create the network socket.   */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof
            serverAddr.sin_zero);

    addr_size = sizeof serverAddr;
    if ( connect( clientSocket, (struct sockaddr *) &serverAddr, 
                addr_size) < 0 )
    {
        printf("[ERROR] Unable to connect to server.\n");
        close( clientSocket );
        exit(0);
    }

    int maxPoints = 1000;
    int counter = 0;
    unsigned int inputLength = 0;

    printf("Enter data to send: ");
    scanf("%[^\n\t]s", userInput);

    inputLength = strlen(userInput);
    if ( send ( clientSocket, userInput, inputLength, 0) != inputLength )
        printf("[ERROR] Unable to send the data.\n");
    /* Read the message from the server into the buffer */
    if ( recv ( clientSocket, buffer, 10, 0) < 0 )
    {
        printf("[ERROR] Connection closed prematurely.\n");
    }

    int availablePoints = maxPoints;
    while ( buffer[0] == '2' )
    {
        ++counter;
        if ( counter < 10 )
        {
            /* Send data to server*/
            printf("Data number %d : ", ( counter + 1 ) );
            printf("Enter the data");
            scanf("%s", userInput);

            inputLength = strlen(userInput);
            if ( send ( clientSocket, userInput, 
                        inputLength, 0) != inputLength )
                printf("[ERROR] Unable to send the data.\n");
            if ( recv ( clientSocket, buffer, 10, 0) < 0 )
                printf("[ERROR] Connection closed prematurely.\n");
        } else {
            userInput = "DONE\0";
            send ( clientSocket, userInput, 5, 0);
            break;
        }
    }
    
    close( clientSocket );
    return 0;
}
