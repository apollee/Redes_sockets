#ifndef PARSE_H
#define PARSE_H 1

/* =============================================================================
 * input_command - input user when starting the program
 * ===========================================================================*/
void input_command(int argc, char *argv[], char *port, char *ip);


/* =============================================================================
 * parse_input_action - parsing the command to perform
 * ===========================================================================*/

int parse_input_action();

/* =============================================================================
 * input_action - executes the command if valid
 * ===========================================================================*/

void input_action(int numTokens, char** saveTokens);

#endif