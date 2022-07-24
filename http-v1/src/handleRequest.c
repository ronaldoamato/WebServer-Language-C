#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "http_server.h"

void *handleRequestFilePath(void* pcli_socket){
    
    int client_connect = *((int*)pcli_socket);          // copia para variavel local e libera o ponteiro
    int client_rsv;
    char message_rsv[MESSAGE_SIZE];
    char *method_http_request,*file_path_request, *file_extension_request;
    
    client_rsv = recv(client_connect, message_rsv, sizeof message_rsv, 0);

    method_http_request = strtok(message_rsv, " \t\n");
    file_path_request = strtok(NULL, " \t");
    file_extension_request = strtok(file_path_request,".");
    file_extension_request = strtok(NULL, " \t");

    if(file_extension_request){
        findFile(client_connect,method_http_request,file_path_request,file_extension_request);
    }
    else{
        file_extension_request="html";
        findFile(client_connect,method_http_request,file_path_request,file_extension_request);
    }


}