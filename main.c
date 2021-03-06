
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
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

int main(){

    sem_init(&mutex, 0, 1);

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
        
        client_connect=accept(server, (struct sockaddr *) &caddr, &csize);
        message_rsv = recv(client_connect, buff, sizeof buff, 0);

        pthread_t thread;
        int *pcli_socket = malloc(sizeof(int));             
        *pcli_socket = client_connect;
                
        if (pthread_create(&thread, NULL, readMessage, pcli_socket)<0){
            printf("Erro ao instanciar nova thread");
        }   // chama a função de tratamento da requisição readMessage

        printf("%s",buff);
        fflush(stdout);
    }

    return 0;
}
