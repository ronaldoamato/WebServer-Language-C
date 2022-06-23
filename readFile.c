#include <stdio.h>  /* defines FILENAME_MAX */
#include <string.h>
#include <stdlib.h>
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


void readFile(int socket) {
    char *buffer;
    char buff[FILENAME_MAX],currentDir[FILENAME_MAX];
    char http_message_200[] = "HTTP/1.0 200 OK\r\nLocation: http://localhost:5000\r\nServer: Apache/0.8.4\r\nContent-Type: text/html\r\n\r\n";
    char http_message_400[] = "HTTP/1.0 400 Bad Request\r\nLocation: http://localhost:5000\r\nServer: Apache/0.8.4\r\nContent-Type: text/html\r\n\r\n";

    GetCurrentDir( currentDir, FILENAME_MAX );
    strcpy(buff,currentDir);
    strcat(buff,"/htdocs/index.html");
    puts(buff);
    FILE *fp;


    fp = fopen(buff, "r");
    if (fp != NULL) {
        fseek(fp, 0, SEEK_END); // tamanho do arquivo
        long bytes_read = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        send(socket, http_message_200 , strlen(http_message_200)+1, 0);
        
        buffer = (char *)malloc(bytes_read * sizeof(char)); 
          
        fread(buffer, bytes_read, 1, fp); // lê o buffer
        write (socket, buffer, bytes_read); //envia html para cliente
        free(buffer);

        fclose(fp);
    }
    else{
        strcpy(buff,currentDir);
        strcat(buff,"/htdocs/400.html");
        fp = fopen(buff, "r");
        fseek(fp, 0, SEEK_END); // tamanho do arquivo
        long bytes_read = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        send(socket, http_message_400 , strlen(http_message_400)+1, 0);
                buffer = (char *)malloc(bytes_read * sizeof(char)); 
          
        fread(buffer, bytes_read, 1, fp); // lê o buffer
        write (socket, buffer, bytes_read); //envia html para cliente
        free(buffer);

        fclose(fp);
    }
}