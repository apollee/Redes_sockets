#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    int fd, errcode;
    socklen_t addrlen;
    ssize_t n;
    char buffer[128];

    //sendUDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket 
    hints.ai_flags = AI_NUMERICSERV;

    errcode = getaddrinfo("lima.tecnico.ulisboa.pt", "58000", &hints, &res);
    if(errcode != 0) //error
        exit(1);

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(fd == -1) //error 
        exit(1);
    
    while (1) {
        n = sendto(fd, "Hello!\n", 7, 0, res->ai_addr, res->ai_addrlen);
        
        if(n == -1) //error
            exit(1);

        printf("Number of characters sent: %ld\n", n);


        //RecvUDP
        addrlen = sizeof(addr);
        n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);

        printf("port: %d\n", addr.sin_port);
        
        if(n == -1) //error
            exit(1);
        
        write(1, "echo: ", 6); //stdout
        write(1,buffer, n);
    }

    freeaddrinfo(res);
    close(fd);
    printf("\n");

    exit(0);
}