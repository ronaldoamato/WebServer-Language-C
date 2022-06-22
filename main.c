
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "http_server.h"

int main(){

    struct sockaddr_in caddr;
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(5001)
    };

    char buff[MESSAGE_SIZE];
    int client_connect, message_rsv,server;
    int csize  = sizeof caddr;

    server=socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1){
        perror("socket failed");
        exit(1);
    }

    if (bind(server, (struct sockaddr *) &saddr, sizeof saddr) == -1){
        perror("bind failed");
        exit(1);
    }

    if(listen(server, MAX_CONNECTIONS) == -1){
        perror("listen failed");
        exit(1);
    }

    while(1){
        client_connect = accept(server, (struct sockaddr *) &caddr, &csize);
        message_rsv = recv(client_connect, buff, sizeof buff, 0);
        
        send(client_connect, buff, client_connect, 0);

        puts(buff);
        fflush(stdout);

        close(client_connect);
    }

    return 0;
}