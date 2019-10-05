#ifndef FS_H
#define FS_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"
#define max(A, B) ((A)>=(B)?(A):(B))

void sigpipe_handler();
void start_UDP();
void start_TCP();
int create_socket(struct addrinfo* res);
//void sendCommandUDP(char* message);
//void sendCommandTCP(char* message);

#endif