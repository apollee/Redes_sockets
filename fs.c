#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define DEFAULT_PORT "58041"
#define FLAG "flag"

/*------------------------*/

int input_command(int argc, char *argv[], char* port);
int createUDPSocket();

/*------------------------*/


int main(int argc, char *argv[]) {
    char port[6];

    strcpy(port, FLAG);
    input_command(argc, argv, port);
    
    if(!strcmp(port, FLAG)){
        strcpy(port, DEFAULT_PORT); //set the default port
    }
    printf("port: %s\n", port);






    struct addrinfo hints,*res;
    int fd;
    ssize_t n;
    char buffer[128];
    struct sockaddr_in addr;
    socklen_t addrlen;

    memset(&hints,0,sizeof hints);

    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_DGRAM;//UDP socket
    hints.ai_flags=AI_NUMERICSERV;

    getaddrinfo(NULL, port, &hints, &res);
    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    n = bind(fd,res->ai_addr,res->ai_addrlen);

    recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);

    printf("%s\n", buffer);

}

int input_command(int argc, char *argv[], char* port) {

    if(argc == 1) {
        return 0;
    }
    else if(argc == 3 && (strcmp(argv[1],"-p") == 0)) {
        strcpy(port, argv[2]);
    }
    else{
        printf("Invalid syntax.\n");
        return -1;
    }
}
