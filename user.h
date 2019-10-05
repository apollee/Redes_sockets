#ifndef USER_H
#define USER_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"

struct addrinfo;
void sigpipe_handler();
void start_UDP();
void start_TCP();
int create_socket(struct addrinfo* res);
void send_commandTCP(char* message);
void send_commandUDP(char* message);
void free_and_close();

#endif