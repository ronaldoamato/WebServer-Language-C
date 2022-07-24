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
#include <errno.h>
#include <ctype.h> 
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>



void * findFile(int cli_socket, char *method_http_request, char *file_path_request,char *file_extension_request) {
    struct timeval  timeval1, timeval2;
    struct timespec req = {0};
    
    
    char *buffer;
    char file_dir[FILENAME_MAX],current_dir[FILENAME_MAX];
    
    char http_message_200[] = "HTTP/1.0 200 OK\r\nLocation: http://localhost:5000\r\nServer: Apache/0.8.4\r\nContent-Type: text/html\r\n\r\n";
    char http_message_400[] = "HTTP/1.0 400 Bad Request\r\nLocation: http://localhost:5000\r\nServer: Apache/0.8.4\r\nContent-Type: text/html\r\n\r\n";
    
    FILE *fp;

    req.tv_sec = 0;  
    
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
        
        fseek(fp, 0, SEEK_END);
        long bytes_read = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        send(cli_socket, http_message_400 , strlen(http_message_400)+1, 0); // Enviando mensagem status 400

        buffer = (char *)malloc(bytes_read * sizeof(char)); 
        fread(buffer, bytes_read, 1, fp); // lê o buffer

        gettimeofday(&timeval1, NULL);
        write (cli_socket, buffer, bytes_read); //envia html para cliente
        gettimeofday(&timeval2, NULL);
        
        double rtt_html = (double) (timeval2.tv_usec - timeval1.tv_usec) / 1000000 + (double) (timeval2.tv_sec - timeval1.tv_sec);
        
        printf("Enviando Request para CLIENT IP: %d RTT: %fs \n",caddr.sin_addr.s_addr,rtt_html);
        
        free(buffer);

        fclose(fp);
        shutdown(cli_socket, SHUT_RDWR);
        close(cli_socket);
        pthread_exit(NULL);
    }
    else {
        fp = fopen(file_dir, "r");
        
        fseek(fp, 0, SEEK_END); // tamanho do arquivo
        long bytes_read = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        send(cli_socket, http_message_200 , strlen(http_message_200)+1, 0);
        buffer = (char *)malloc(bytes_read * sizeof(char)); 
          
        fread(buffer, bytes_read, 1, fp); // lê o buffer
        gettimeofday(&timeval1, NULL);
        write (cli_socket, buffer, bytes_read); //envia html para cliente
        gettimeofday(&timeval2, NULL);
        
        double rtt_html = (double) (timeval2.tv_usec - timeval1.tv_usec) / 1000000 + (double) (timeval2.tv_sec - timeval1.tv_sec);
        printf("Enviando Request para CLIENT IP: 192.168.0.1 RTT: %fs \n",rtt_html);
        
        free(buffer);

        fclose(fp);
        shutdown(cli_socket, SHUT_RDWR);
        close(cli_socket);
        pthread_exit(NULL);
    }
}
