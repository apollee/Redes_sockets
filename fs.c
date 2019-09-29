#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "parse.h"

/*------------------------*/

int createSocket(struct addrinfo* res);

/*------------------------*/


int main(int argc, char *argv[]) {
    struct addrinfo hintsUDP,*resUDP;
    struct addrinfo hintsTCP,*resTCP;
    int fdUDP, fdTCP;
    ssize_t n;
    char buffer[128];
    char port[6];
    struct sockaddr_in addr;
    socklen_t addrlen;
    extern int errno;

    input_command_server(argc, argv, port);
    printf("port: %s\n", port);

    //UDP--------------------------------------------------
    memset(&hintsUDP,0,sizeof hintsUDP);
    hintsUDP.ai_family=AF_INET;//IPv4
    hintsUDP.ai_socktype=SOCK_DGRAM;//UDP socket
    hintsUDP.ai_flags=AI_PASSIVE|AI_NUMERICSERV;

    getaddrinfo(NULL, port, &hintsUDP, &resUDP);

    fdUDP = createUDPSocket(resUDP);
    n = bind(fdUDP,resUDP->ai_addr,resUDP->ai_addrlen);

    addrlen=sizeof(addr);
    n = recvfrom(fdUDP, buffer, 128, 0,(struct sockaddr*)&addr,&addrlen);
    
    printf("%s\n", buffer);

    n = sendto(fdUDP, buffer, n, 0, (struct sockaddr*)&addr, addrlen);

    //TCP-----------------------------------------
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_PASSIVE|AI_NUMERICSERV;

    getaddrinfo(NULL, port, &hintsTCP, &resTCP);

    fdTCP = createUDPSocket(resTCP);
    n = bind(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    memset(buffer, 0, strlen(buffer));
    listen(fdTCP, 5);
    
    int newfd = accept(fdTCP, (struct sockaddr*)&addr, &addrlen);
    printf("%d", newfd);
    
    int b = read(newfd, buffer, 128);
    write(1, "received: \n", 11);
    write(1, buffer, b); //fs

    b = write(newfd, buffer, b); 
}


int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}