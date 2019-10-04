#ifndef COMMANDS_USER_H
#define COMMANDS_USER_H 1

#define TRUE 1
#define FALSE 0

int parse_command(char* message);
int onlyNumbers(char* message);
void command_received(int numTokens, char** saveTokens, char* input, long int numberChar);
int commandREGOK(int numTokens, char** saveTokens, long int numberChar);
int commandLTPOK(int numTokens, char** saveTokens, long int numberChar);
int commandPTPOK(int numTokens, char** saveTokens, long int numberChar);
int commandLQROK(int numTokens, char** saveTokens, long int numberChar);
int commandQGROK(int numTokens, char** saveTokens, long int numberChar);
int commandQUROK(int numTokens, char** saveTokens, long int numberChar);
int commandANROK(int numTokens, char** saveTokens, long int numberChar);




#endif