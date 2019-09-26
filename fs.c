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

    struct addrinfo hintsUDP,*resUDP;
    struct addrinfo hintsTCP,*resTCP;
    int fdUDP, fdTCP;
    ssize_t n;
    char buffer[128];
    struct sockaddr_in addr;
    socklen_t addrlen;

    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_PASSIVE|AI_NUMERICSERV;

    memset(&hintsUDP,0,sizeof hintsUDP);
    hintsUDP.ai_family=AF_INET;//IPv4
    hintsUDP.ai_socktype=SOCK_DGRAM;//UDP socket
    hintsUDP.ai_flags=AI_PASSIVE|AI_NUMERICSERV;

    getaddrinfo(NULL, port, &hintsTCP, &resUDP);
    getaddrinfo(NULL, port, &hintsUDP, &resUDP);

    fdTCP = socket(resTCP->ai_family, resTCP->ai_socktype, resTCP->ai_protocol);

    fdUDP = socket(resUDP->ai_family, resUDP->ai_socktype, resUDP->ai_protocol);

    n = bind(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    
    n = bind(fdUDP,resUDP->ai_addr,resUDP->ai_addrlen);

    addrlen=sizeof(addr);
    n = recvfrom(fdUDP, buffer, 128, 0,(struct sockaddr*)&addr,&addrlen);
    printf("%s\n", buffer);

    sendto(fdUDP, "Ola!\n" , 5, 0, (struct sockaddr*)&addr,addrlen);
   
    
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
