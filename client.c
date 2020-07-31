#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "wrap_unwrap.h"

int connectSocket(char *host, int port)
{
    int frontBackSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (frontBackSocket < 0)
    {
        perror("Client Error: Socket not created succesfully");
        return -1;
    }
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    int result = inet_pton(AF_INET, host, &server.sin_addr);
    if (result < 0)
    {
        perror("Client Error: IP not initialized succesfully");
        return -1;
    }
    result = connect(frontBackSocket, (struct sockaddr *)&server, sizeof(server)); //Connect to given server
    if (result < 0)
    {
        perror("Client Error: Connection Failed.");
        return -1;
    }
    return frontBackSocket;
}

int client (int argc, char *argv[])
{
    char buff[bufLengs];
    int frontSocket = -1;
    while (frontSocket < 0)
        frontSocket = connectSocket(argv[3], atoi(argv[2]));
    printf("Front connected");
    int numberBytes;
    int backSocket = -1;
    while(1)
    {
        numberBytes = recv(frontSocket, buff, bufLengs, MSG_DONTWAIT);
        if (numberBytes > 0)
        {
            printf("Received from front: %s\n", buff);
            if (buff[0] == 'c')
                backSocket = connectSocket(argv[5], atoi(argv[4]));
            else if (buff[0] == 'd')
            {
                unwrap_data(buff, &numberBytes);
                printf("Sending to back: %s\n", buff);
                if (backSocket != -1)
                    send(backSocket, buff, numberBytes, 0); //Send the reversed input
            }
            else
            {
                printf("Close signal received from server");
                return 0;
            }
        }
        else if (numberBytes < 0 && (errno != EAGAIN && errno != EWOULDBLOCK))
        {
            printf("Back read fail\n");
            break ;
        }
        if (backSocket != -1)
        {
            numberBytes = recv(backSocket, buff, bufLengs - hederLeng, MSG_DONTWAIT);
            if (numberBytes > 0)
            {
                printf("Received from back: %s", buff);
                wrap_data(buff, &numberBytes);
                printf("Sending to back: %s", buff);
                send(frontSocket, buff, numberBytes, 0); //Send the reversed input
            }
            else if (numberBytes < 0 && (errno != EAGAIN && errno != EWOULDBLOCK))
            {
                printf("Front read fail\n");
                break ;
            }
        }
    }
    close(frontSocket);
    return 0;
}
