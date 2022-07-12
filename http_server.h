#define MAX_CONNECTIONS 5
#define MESSAGE_SIZE 2048
#define HTTP_PORT 50000
    
//DECLARACAO DA FUNCOES
int initServer(struct sockaddr_in caddr,struct sockaddr_in saddr);
//void readFile(int socket);
void * readFile(void* pcli_socket);
void readRequest(char *message, void *pcli_socket);
