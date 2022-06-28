
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

    struct sockaddr_in caddr;
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(HTTP_PORT)
    };

    char buff[MESSAGE_SIZE];
    int client_connect, message_rsv, server;
    int csize  = sizeof caddr;

    server = initServer(caddr,saddr);

    while(1){
        printf ("Aguardando conexões...\n");
        if((client_connect = accept(server, (struct sockaddr *) &caddr, &csize))==true){
            printf ("Conectado!");
            message_rsv = recv(client_connect, buff, sizeof buff, 0);
        //readFile(client_connect);

            pthread_t thread;                                       //instancia uma thread caso receba um pedido de conexão
            int *pcli_socket = malloc(sizeof(int));                 // aloca em heap 
            *pcli_socket = client_connect;                          // aloca o socket
            pthread_create(&thread, NULL, readFile, pcli_socket);   // chama a função de tratamento da requisição readFile
        }
        //send(client_connect, buff, client_connect, 0);
        puts(buff);
        fflush(stdout);

        close(client_connect);
    }

    return 0;
}
