#ifndef COMMANDS_USER_H
#define COMMANDS_USER_H 1

#define TRUE 1
#define FALSE 0

char local_topic[120];
char id_user[5];

int isREG(char* id_user);
int onlyNumbers(char* message);
void send_message_reg(char* id_user, char* message);
void send_message_tl(char* message);
void send_message_tp(char* topic, char* message);
void send_message_ql(char* message);
void send_message_qg(char* message);
void send_message_qs(char* message, int numTokens, char** saveTokens);
void send_message_as(char* message);
void send_message_err(char* message);
void questions_print(char** saveTokens);
void topics_print(char** saveTokens);
int commandREGOK(int numTokens, char** saveTokens, long int numberCar);
int commandTLOK(int numTokens, char ** saveTokens, long int numberCar);
int commandTSOK(int numTokens, char** saveTokens, long int numberCar);
int commandTPOK(int numTokens, char** saveTokens, long int numberCar);
int commandQLOK(int numTokens, char ** saveTokens, long int numberCar);
int commandQGOK(int numTokens, char** saveTokens, long int numberCar);
int commandQSOK(int numTokens, char** saveTokens, long int numberCar);
int commandASOK(int numTokens, char** saveTokens, long int numberCar);


#endif