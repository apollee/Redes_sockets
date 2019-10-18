#ifndef PARSE_USER_H
#define PARSE_USER_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"


void input_command_user(int argc, char *argv[], char *port, char *ip);
void input_action(int numTokens, char** saveTokens, char* input, long int numberCar);
int parse_command();
void parse_command_received(char* buffer);
char** parse_command_received_TCP(char* buffer);
void input_action_received(int numTokens, char** saveTokens, long int numberChar);
int treatBufferData2(int ind, int num, char* buffer);
int treatBufferData(char** saveTokens, int ind, int num, char* buffer);
int parse_image_qg(int indice, char* buffer);
int treatBufferImg(int ind, int num, long int n, char* buffer, char* ext);
char** saveTokensInit(int row, int collumn);
int parse_image_qg(int indice, char* buffer);
int parse_answers_qg(int indice, char* buffer);
int parse_answers_image_qg(int indice, char* buffer);

void input_action_received_TCP(char** saveTokens);

#endif