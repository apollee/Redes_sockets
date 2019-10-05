#ifndef FS_H
#define FS_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"
#define max(A, B) ((A)>=(B)?(A):(B))

int createSocket(struct addrinfo* res);
int input_command_server(int argc, char *argv[], char* port);
//void sendCommandUDP(char* message);
//void sendCommandTCP(char* message);

#endif