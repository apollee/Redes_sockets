#ifndef PARSE_H
#define PARSE_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"

/* =============================================================================
 * input_command_user - input user when starting the program
 * ===========================================================================*/

void input_command_user(int argc, char *argv[], char *port, char *ip);

/* =============================================================================
 * input_command_server - input server when starting the program
 * ===========================================================================*/

int input_command_server(int argc, char *argv[], char* port);

/* =============================================================================
 * parse_input_action - parsing the command to perform
 * ===========================================================================*/

int parse_input_action();

/* =============================================================================
 * input_action - executes the command if valid
 * ===========================================================================*/

void input_action(int numTokens, char** saveTokens, char* input);

#endif