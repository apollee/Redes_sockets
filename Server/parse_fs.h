#ifndef PARSE_FS_H
#define PARSE_FS_H 1

#define DEFAULT_PORT "58041"

int input_command_server(int argc, char *argv[], char *port);
char* input_action(int numTokens, char** saveTokens, char* input, long int numberCar, const char* ip);
char* parse_command(char* message, const char* ip);
char** parse_commandTCP(char* message);
int treatBufferData(char** saveTokens, int ind, int num, char* buffer);
int treatBufferImg(char** saveTokens, int ind, int num, char* buffer);
char** parse_commandTCPImg(int i, char* message);
int isREG(const char* ip);
char** saveTokensInit(int row, int collunm);
#endif