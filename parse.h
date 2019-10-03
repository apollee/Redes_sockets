#ifndef PARSE_H
#define PARSE_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"


void input_command_user(int argc, char *argv[], char *port, char *ip);

void input_action(int numTokens, char** saveTokens, char* input, long int numberCar);

int parse_input_action();

#endif