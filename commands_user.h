#ifndef COMMANDS_USER_H
#define COMMANDS_USER_H 1

#define TRUE 1
#define FALSE 0

int onlyNumbers(char* message);

int commandREGOK(int numTokens, char** saveTokens, long int numberCar);
int commandTLOK(int numTokens, char ** saveTokens, long int numberCar);
int commandTPOK(int numTokens, char** saveTokens, long int numberCar);
int commandTSOK(int numTokens, char** saveTokens, long int numberCar);
int commandQLOK(int numTokens, char ** saveTokens, long int numberCar);
int commandQGOK(int numTokens, char** saveTokens, long int numberCar);
int commandQSOK(int numTokens, char** saveTokens, long int numberCar);
int commandASOK(int numTokens, char** saveTokens, long int numberCar);

#endif