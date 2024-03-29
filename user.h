#ifndef USER_H
#define USER_H 1

#define FLAG "flag"
#define DEFAULT_PORT "58041"
#define DEFAULT_BUFFER_SIZE 1024


struct addrinfo *resTCP;
int fdTCP;

struct addrinfo;
void sigpipe_handler();
void initialize_flags();
void start_UDP();
void start_TCP();
int create_socket(struct addrinfo* res);
void send_commandUDP(char* message);
void send_commandTCP(char* message);
int connectTCP();
int writeTCP(char* message, int nread);
char* readTCP();
//char* readTCP(char* buffer);

void free_and_close();

#endif