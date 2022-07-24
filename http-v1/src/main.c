#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "http_server.h"

int main(){

    caddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(HTTP_PORT);

    int client_connect, server;
    int csize  = sizeof caddr;

    server = initServer(caddr,saddr);

    while(1){
        printf("Aguardando conexões...\n");
        
        client_connect=accept(server, (struct sockaddr *) &caddr, &csize);
        
        int *pcli_socket = malloc(sizeof(int));             
        *pcli_socket = client_connect;

        handleRequestFilePath(pcli_socket);
        
        fflush(stdout);
    }

    return 0;
}
