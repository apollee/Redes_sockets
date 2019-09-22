#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <string.h> 
#include <stdio.h>

int main(void){
    struct addrinfo hints, *res;
    int fd, errcode;
    ssize_t n;
    char message[50];
    int total = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket 
    hints.ai_flags = AI_NUMERICSERV;

    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res);
    if(errcode != 0) //error
        exit(1);

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(fd == -1) //error 
        exit(1);
    while (1){
        scanf("%s", message);
        n = sendto(fd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen);
        if(n == -1) //error
            exit(1);
        total += n;
        if (!strcmp(message,"exit"))
            break;    
    }
    
    

    printf("Number of characters sent: %ld\n", total);
    freeaddrinfo(res);
    printf("\n");

    exit(0);
}