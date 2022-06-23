#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "http_server.h"

int initServer(struct sockaddr_in caddr, struct sockaddr_in saddr){

    int server=socket(AF_INET, SOCK_STREAM, 0);

    if (server == -1){
        perror("socket failed");
        exit(1);
    };

    if (bind(server, (struct sockaddr *) &saddr, sizeof saddr) == -1){
        perror("bind failed");
        exit(1);
    };

    if(listen(server, MAX_CONNECTIONS) == -1){
        perror("listen failed");
        exit(1);
    };

    return server;
};