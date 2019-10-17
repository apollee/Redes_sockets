#ifndef COMMANDS_USER_H
#define COMMANDS_USER_H 1

#define TRUE 1
#define FALSE 0

int onlyNumbers(char* message);
char* command_received(int numTokens, char** saveTokens, char* input, long int numberChar);
int commandREGOK(int numTokens, char** saveTokens, long int numberChar);
int commandLTPOK(int numTokens, char** saveTokens, long int numberChar);
int commandPTPOK(int numTokens, char** saveTokens, long int numberChar);
int commandLQUOK(int numTokens, char** saveTokens, long int numberChar);
int commandGQUOK(char** saveTokens, long int numberChar);
int commandQUSOK(char** saveTokens, int numberChar);
int commandANSOK(char** saveTokens, long int numberChar);
char* checkTopics();
char* checkQuestions(char** saveTokens);
char* proposeTopic(char** saveTokens);
char* checkSubmitQuestion(char** saveTokens);

#endif