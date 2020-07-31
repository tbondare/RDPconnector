#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "wrap_unwrap.h"

int init_accept_socket(int port)
{
    int acceptSocket = 0;
    acceptSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(acceptSocket < 0)
    {
        perror("Client Error: Socket not created succesfully");
        return 0;
    }
    //Structure to store details
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));
    //Initialize
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(acceptSocket, (struct sockaddr*)&server, sizeof(server));
    listen(acceptSocket, 1);
    return acceptSocket;
}

char unwrap_data(char *buff, int *numberBytes)
{
    if (buff[0] == 'd')
    {
        *numberBytes = atoi(&buff[1]);
        memmove(&buff[0], &buff[hederLeng], *numberBytes);
    }
    return buff[0];
}

void wrap_data(char *buff, int *numberBytes)
{

    memmove(&buff[hederLeng], &buff[0], *numberBytes);
    buff[0] = 'd';
    sprintf(&buff[1], "%5d", *numberBytes);
    buff[hederLeng - 1] = '\0';
    *numberBytes = *numberBytes + hederLeng;
}

int server(int argc, char *argv[])
{
    int backAcceptSocket = init_accept_socket(atoi(argv[3]));
    int frontAcceptSocket = init_accept_socket(atoi(argv[2]));
    char buff[bufLengs];
    int backConnectionSocket = accept(backAcceptSocket, (struct sockaddr *) NULL, NULL);
    while(backConnectionSocket)
    {
        printf("\nBack connected\n");
        int frontConnectionSocket = accept(frontAcceptSocket, (struct sockaddr *) NULL, NULL);
        while (frontConnectionSocket)
        {
            printf("Front connected\n");
            buff[0] = 'c';
            send(backConnectionSocket, buff, 1, 0);
            int numberBytes;
            while (1)
            {
                numberBytes = recv(backConnectionSocket, buff, bufLengs, MSG_DONTWAIT);
                if (numberBytes > 0)
                {
                    printf("Server Received from back: %s", buff);
                    unwrap_data(buff, &numberBytes);
                    printf("Server Sending to front: %s",buff);
                    send(frontConnectionSocket, buff, numberBytes, 0); //Send the reversed input
                }
                else if (numberBytes < 0 && (errno != EAGAIN && errno != EWOULDBLOCK))
                {
                    printf("Back read fail\n");
                    break ;
                }
                numberBytes = recv(frontConnectionSocket, buff, bufLengs - hederLeng, MSG_DONTWAIT);
                if (numberBytes > 0)
                {
                    printf("Server Received from front: %s", buff);
                    wrap_data(buff, &numberBytes);
                    printf("Server Sending to back: %s", buff);
                    send(backConnectionSocket, buff, numberBytes, 0); //Send the reversed input
                }
                else if (numberBytes < 0 && (errno != EAGAIN && errno != EWOULDBLOCK))
                {
                    printf("Front read fail\n");
                    break ;
                }
            }
            close(backConnectionSocket);
            close(frontConnectionSocket);
            frontConnectionSocket = accept(frontAcceptSocket, (struct sockaddr *) NULL, NULL);
        }
        backConnectionSocket = accept(backAcceptSocket, (struct sockaddr *) NULL, NULL);
    }
}
