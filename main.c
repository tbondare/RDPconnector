#include "server.h"
#include "client.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 6 && argc != 4)
    {
        printf("wrong number of arguments. See readme for usage");
        return 1;
    }
    char fl = argv[1][0];
    if (fl == 's')
        server(argv);
    else if (fl == 'c')
        client(argv);
    else
    {
        printf("wrong argument. use s or c like an argument");
        return 0;
    }
}