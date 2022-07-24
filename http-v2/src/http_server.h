#define MAX_CONNECTIONS 5
#define MESSAGE_SIZE 2048
#define HTTP_PORT 50002

struct sockaddr_in caddr, saddr;

//DECLARACAO DA FUNCOES
int initServer(struct sockaddr_in caddr,struct sockaddr_in saddr);
void *findFile(int cli_socket, char *method_http_request, char *file_path_request, char *file_extension_request);
void *handleRequestFilePath(void* pcli_socket);