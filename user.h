#ifndef USER_H
#define USER_H 1

struct addrinfo;

void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip);

int createSocket(struct addrinfo* res);

void sendCommandTCP(char* message);

void sendCommandUDP(char* message);

#endif