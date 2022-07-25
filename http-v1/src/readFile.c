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
    req.tv_sec = 0; 
    
    char file_dir[FILENAME_MAX],current_dir[FILENAME_MAX];
    char header_buff [250];
    char *buffer;

    long bytes_read;

    FILE *fp;
    
    GetCurrentDir(current_dir, FILENAME_MAX);
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

    if(strcmp(file_extension_request,"html") == 0 ){
        fp = fopen(file_dir, "r");
    
        if (fp == NULL) {
            strcpy(file_dir,current_dir);
            strcat(file_dir,"/htdocs/400.html");
            strcpy (header_buff, "HTTP/1.1 400 Bad Request\r\n");
            strcat (header_buff, "Content-type: text/html\r\n");
            strcat (header_buff, "Location: ");
            strcat (header_buff, inet_ntoa(saddr.sin_addr));
            strcat (header_buff, "\r\n");
            strcat (header_buff, "Server: ");
            strcat (header_buff, SERVER_NAME);
            strcat (header_buff, "\r\n");
            strcat (header_buff, "Connection: keep-alive\r\n\r\n");

            send(cli_socket, header_buff , strlen(header_buff)+1, 0); // Enviando mensagem status 400
    
            buffer=fileLength(file_dir,&bytes_read);

            gettimeofday(&timeval1, NULL);
            write (cli_socket, buffer, bytes_read); //envia html para cliente
            gettimeofday(&timeval2, NULL);
            
            double rtt_html = (double) (timeval2.tv_usec - timeval1.tv_usec) / 1000000 + (double) (timeval2.tv_sec - timeval1.tv_sec);
            
            printf("Enviando Request para CLIENT IP: %d RTT: %fs \n",caddr.sin_addr.s_addr,rtt_html);
            
            free(buffer);
            free(fp);
            shutdown(cli_socket, SHUT_RDWR);
            close(cli_socket);
        }
        else{
            strcpy (header_buff, "HTTP/1.1 200 OK\r\n");
            strcat (header_buff, "Content-type: text/html\r\n");
            strcat (header_buff, "Location: ");
            strcat (header_buff, inet_ntoa(saddr.sin_addr));
            strcat (header_buff, "\r\n");
            strcat (header_buff, "Server: ");
            strcat (header_buff, SERVER_NAME);
            strcat (header_buff, "\r\n");
            strcat (header_buff, "Connection: keep-alive\r\n\r\n");
            
            send(cli_socket, header_buff , strlen(header_buff)+1, 0); // Enviando mensagem status 400
    
            buffer=fileLength(file_dir,&bytes_read);

            gettimeofday(&timeval1, NULL);
            write (cli_socket, buffer, bytes_read); //envia html para cliente
            gettimeofday(&timeval2, NULL);
            
            double rtt_html = (double) (timeval2.tv_usec - timeval1.tv_usec) / 1000000 + (double) (timeval2.tv_sec - timeval1.tv_sec);
            
            printf("Enviando Request para CLIENT IP: %s RTT: %fs \n",inet_ntoa(caddr.sin_addr),rtt_html);
            
            free(buffer);
            free(fp);
            shutdown(cli_socket, SHUT_RDWR);
            close(cli_socket);
        }
    }
    else{
        
        struct stat filestat;
        int fd;
        char file_buff [MAX_FILE_BUFF];
        char filesize[10];//, name[30]; 

        
        printf("%s",file_dir);
        if ( ((fd = open (file_dir, O_RDONLY)) < -1) || (fstat(fd, &filestat) < 0) ) {
            printf ("Error in measuring the size of the file\n");
            strcpy(file_dir,current_dir);
            strcat(file_dir,"/htdocs/400.html");
            fp = fopen ("file_dir", "r");
        }

        if (file_dir == NULL) {
            printf("arquivo não aberto\n");
        }

        sprintf (filesize, "%zd", filestat.st_size);
        fp = fopen (file_dir, "r");
    
        if (fp == NULL) {
            printf ("fp is null or filename = 404\n");
            strcpy(file_dir,current_dir);
            strcat(file_dir,"/htdocs/400.html");
            fp = fopen ("file_dir", "r");
        }

        else if (fp != NULL) {
            strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
            /* content-length: */
            strcat (header_buff, filesize);
            strcat (header_buff, "\r\n");
            /* content-type: */
            strcat (header_buff, "content-type: image/png\r\n");
        }

        else {
            strcpy(file_dir,current_dir);
            strcat(file_dir,"/htdocs/503.html");
            strcpy (header_buff, "HTTP/1.1 503 INTERNAL SERVER ERROR\r\nContent-Length: ");
            /* content-length: */
            strcat (header_buff, filesize);
            strcat (header_buff, "\r\n");
            /* content-type: */
            strcat (header_buff, "content-type: image/png\r\n");
            fp = fopen ("file_dir", "r");
        }        

        strcat (header_buff, "Location: ");
        strcat (header_buff, inet_ntoa(saddr.sin_addr));
        strcat (header_buff, "\r\n");
        strcat (header_buff, "Server: ");
        strcat (header_buff, SERVER_NAME);
        strcat (header_buff, "\r\n");
        strcat (header_buff, "Connection: keep-alive\r\n\r\n");

        write (cli_socket, header_buff, strlen(header_buff));

        fread (file_buff, sizeof(char), filestat.st_size + 1, fp);
        fclose(fp);
        write (cli_socket, file_buff, filestat.st_size);
        shutdown(cli_socket, SHUT_RDWR);
        close (cli_socket);
    }
}

char* fileLength(char *file_dir, long* bytes_read){
    FILE *fp;
    char *buffer_file;

    fp = fopen(file_dir, "r");
          
    fseek(fp, 0, SEEK_END);
    long bytes_read_num = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    buffer_file = (char *)malloc(bytes_read_num * sizeof(char));
    *bytes_read=bytes_read_num;
    
    fread(buffer_file, bytes_read_num, 1, fp); // lê o buffer
    fclose(fp);
    return buffer_file;
}