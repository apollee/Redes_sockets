#ifndef COMMANDS_USER_H
#define COMMANDS_USER_H 1

#define TRUE 1
#define FALSE 0

int parse_command(char* message);
int onlyNumbers(char* message);
void command_received(int numTokens, char** saveTokens, char* input, long int numberCar);

#endif