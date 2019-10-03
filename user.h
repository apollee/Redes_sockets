#ifndef USER_H
#define USER_H 1

struct addrinfo;

void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip);

int createSocket(struct addrinfo* res);

void sendCommandTCP(char* message);

void sendCommandUDP(char* message);

int commandREGOK(int numTokens, char** saveTokens, long int numberCar);

int commandTLOK(int numTokens, char ** saveTokens, long int numberCar);

int commandTPOK(int numTokens, char** saveTokens, long int numberCar);

int commandTSOK(int numTokens, char** saveTokens, long int numberCar);

int commandQLOK(int numTokens, char ** saveTokens, long int numberCar);

int commandQGOK(int numTokens, char** saveTokens, long int numberCar);

int commandQSOK(int numTokens, char** saveTokens, long int numberCar);

int commandASOK(int numTokens, char** saveTokens, long int numberCar);

#endif