#ifndef PARSE_FS_H
#define PARSE_FS_H 1

#define DEFAULT_PORT "58041"

int input_command_server(int argc, char *argv[], char *port);
char* input_action(int numTokens, char** saveTokens, char* input, long int numberCar, const char* ip);
char* parse_command(char* message, const char* ip);
char** parse_commandTCP(char* message);
int treatBufferData(char** saveTokens, int ind, int num, char* buffer);
int treatBufferImg(char** saveTokens, int ind, int num, long int n, char* buffer, char* ext);
char** parse_commandTCPImg(int i, char* message);
int isREG(const char* ip);
char** saveTokensInit(int row, int collunm);
void parseQGU(char** saveTokens, int socket, char* buffer);
int treatBufferDataQGU(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char*perm);
int treatBufferDataImgQGU(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char*perm);
char* parseANS(char**saveTokens, int newfd, ssize_t n, char* buffer, char* message);
int treatBufferDataANS(char** saveTokens, int ind, int num, char* buffer, char* number);
int treatBufferImgANS(char** saveTokens,int ind, int num, long int n, char* buffer, char* ext, char* number);
int treatBufferDataImgQGUANS(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char* perm, char* numbAns);
int treatBufferDataQGUANS(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char* perm, char* numbAns);
#endif