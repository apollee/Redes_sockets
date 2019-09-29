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

void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip);
int createSocket(struct addrinfo* res);

/*------------------------*/


int main(int argc, char *argv[]) {
     struct addrinfo hintsUDP, hintsTCP;
    struct addrinfo *resUDP, *resTCP;
    struct sockaddr_in addr;
    socklen_t addrlen;
    ssize_t n;
    char host_name[128];
    char buffer[128];
    char buffer1[128];
    char port[6];
    char ip[INET_ADDRSTRLEN];
    int fdUDP, fdTCP;

    gethostname(host_name,128);
    input_command_user(argc, argv, port, ip);

    //getIp(hintsUDP, host_name, port, resUDP, ip);
    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;
  
    getaddrinfo(host_name, port, &hintsUDP, &resUDP);
    
    if(!strcmp(ip, FLAG)){
        inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip);
    }
    
    fdUDP = createSocket(resUDP);
    sendto(fdUDP,"Hello!\n",7,0,resUDP->ai_addr,resUDP->ai_addrlen);

    addrlen = sizeof(addr);
    n = recvfrom(fdUDP, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
    write(1, buffer, n);


    //TCP-------------------------------------------------------------------
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_NUMERICSERV;
    SO_REUSEPORT;

    getaddrinfo(NULL, port, &hintsTCP, &resTCP);

    fdTCP = createSocket(resTCP);
    int h = connect(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    printf("%d", h);
    
    int b = write(fdTCP, "ola\n", 4);
    b = read(fdTCP, buffer1, 128);

    write(1, "echo: ", 6);
    write(1, buffer1, b);

    while(1){
        parse_input_action();
    }
}


//Not being used----------------------------
void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip){
    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;

    int errcode = getaddrinfo(host_name, port, &hintsUDP, &resUDP);
    
    if(!strcmp(ip, FLAG)){
        inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip);
    }
}


int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}
