#ifndef PARSE_FS_H
#define PARSE_FS_H 1

#define DEFAULT_PORT "58041"

Link head;

int input_command_server(int argc, char *argv[], char *port);
char* input_action(int numTokens, char** saveTokens, char* input, long int numberCar, const char* ip);
char* parse_command(char* message, const char* ip);

#endif