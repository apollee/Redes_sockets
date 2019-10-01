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

#define max(A, B) ((A)>=(B)?(A):(B))

/*------------------------*/

int createSocket(struct addrinfo* res);

/*------------------------*/


int main(int argc, char *argv[]) {
    struct addrinfo hintsUDP,*resUDP;
    struct addrinfo hintsTCP,*resTCP;
    int fdUDP, fdTCP, errcode, newfd;
    ssize_t n;
    char buffer[128];
    char port[6];
    struct sockaddr_in addr;  
    socklen_t addrlen;
    extern int errno;
    fd_set rfds;
    int maxDescriptor, counter;
    enum {idle, busy} state;

    input_command_server(argc, argv, port);
    printf("port: %s\n", port);

    //UDP--------------------------------------------------
    memset(&hintsUDP,0,sizeof hintsUDP);
    hintsUDP.ai_family=AF_INET;//IPv4
    hintsUDP.ai_socktype=SOCK_DGRAM;//UDP socket
    hintsUDP.ai_flags=AI_PASSIVE|AI_NUMERICSERV;

    if ((errcode = getaddrinfo(NULL, port, &hintsUDP, &resUDP)) != 0){
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    }

    fdUDP = createSocket(resUDP);
    if(fdUDP == -1){
        printf("creating Server UDP socket failed\n");
    }

    n = bind(fdUDP,resUDP->ai_addr,resUDP->ai_addrlen);
    if(n == -1){
        printf("bind not working Server UDP\n");
    }
    
    addrlen=sizeof(addr);
    n = recvfrom(fdUDP, buffer, 128, 0,(struct sockaddr*)&addr,&addrlen);
    if(n == -1){
        printf("recv from not working Server UDP\n");
    }
    
    write(1, "received UDP: ", 15);
    write(1, buffer, n);

    n = sendto(fdUDP, buffer, n, 0, (struct sockaddr*)&addr, addrlen);
    if(n == -1){
        printf("send to not working Server UDP\n");
    }

    close(fdUDP);

    //TCP-----------------------------------------
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_PASSIVE|AI_NUMERICSERV;

    if((errcode = getaddrinfo(NULL, port, &hintsTCP, &resTCP)) != 0){
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    }

    fdTCP = createSocket(resTCP);
    if(fdTCP == -1){
        printf("creating Server TCP socket failed\n");
    }

    n = bind(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    if(n == -1){
        printf("bind not working Server TCP\n");
    }

    memset(buffer, 0, strlen(buffer));
    n = listen(fdTCP, 5);
    if(n == -1){
        printf("listen not working Server TCP\n");
    }
    
    if ((newfd = accept(fdTCP, (struct sockaddr*)&addr, &addrlen)) == -1){
        fprintf(stderr, "error: newfd: %s\n", gai_strerror(newfd));
    };
    
    int b = read(newfd, buffer, 128);
    if(b == -1){
        printf("read not working Server TCP\n");
    }

    write(1, "received TCP: ", 15);
    write(1, buffer, b); 

    b = write(newfd, buffer, b);
    if(b == -1){
        printf("write not working Server TCP\n");
    }
    close(fdTCP);
}


int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}

