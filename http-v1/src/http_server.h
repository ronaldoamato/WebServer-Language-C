#define MAX_CONNECTIONS 5
#define MESSAGE_SIZE 2048
#define HTTP_PORT 50001
#define MAX_FILE_BUFF 2136038
#define SERVER_NAME "server-redes"

struct sockaddr_in caddr, saddr;

//DECLARACAO DA FUNCOES
int initServer(struct sockaddr_in caddr,struct sockaddr_in saddr);
void *findFile(int cli_socket, char *method_http_request, char *file_path_request, char *file_extension_request);
char* fileLength(char *file_dir, long* bytes_read);
void *handleRequestFilePath(void* pcli_socket);