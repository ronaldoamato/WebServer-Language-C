#include <stdio.h>  /* defines FILENAME_MAX */
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "http_server.h"
#include <pthread.h>



void * findFile(int cli_socket, char *method_http_request, char *file_path_request,char *file_extension_request) {
        
    char *buffer;
    char file_dir[FILENAME_MAX],current_dir[FILENAME_MAX];
    
    char http_message_200[] = "HTTP/1.0 200 OK\r\nLocation: http://localhost:5000\r\nServer: Apache/0.8.4\r\nContent-Type: text/html\r\n\r\n";
    char http_message_400[] = "HTTP/1.0 400 Bad Request\r\nLocation: http://localhost:5000\r\nServer: Apache/0.8.4\r\nContent-Type: text/html\r\n\r\n";
    FILE *fp;

    GetCurrentDir( current_dir, FILENAME_MAX );
    strcpy(file_dir,current_dir);
    strcat(file_dir,"/htdocs");

    if(strcmp(file_path_request,"/")==0){
        strcat(file_dir,"/index.html");
    }
    else{
        strcat(file_dir,file_path_request);
        strcat(file_dir,".");
        strcat(file_dir,file_extension_request);
    }

    fp = fopen(file_dir, "r");

    if (fp == NULL) {
        strcpy(file_dir,current_dir);
        strcat(file_dir,"/htdocs/400.html");
        
        fp = fopen(file_dir, "r");
        
        fseek(fp, 0, SEEK_END); // tamanho do arquivo
        long bytes_read = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        send(cli_socket, http_message_400 , strlen(http_message_400)+1, 0);
        buffer = (char *)malloc(bytes_read * sizeof(char)); 
          
        fread(buffer, bytes_read, 1, fp); // lê o buffer
        write (cli_socket, buffer, bytes_read); //envia html para cliente
        free(buffer);

        fclose(fp);
        close(cli_socket);
    }
    else {
    
        fp = fopen(file_dir, "r");
        
        fseek(fp, 0, SEEK_END); // tamanho do arquivo
        long bytes_read = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        send(cli_socket, http_message_200 , strlen(http_message_200)+1, 0);
        buffer = (char *)malloc(bytes_read * sizeof(char)); 
          
        fread(buffer, bytes_read, 1, fp); // lê o buffer
        write (cli_socket, buffer, bytes_read); //envia html para cliente
        free(buffer);

        fclose(fp);
        close(cli_socket);
    }
}
