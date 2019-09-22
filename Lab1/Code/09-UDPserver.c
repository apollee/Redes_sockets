#include <stdlib.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(void){
    struct addrinfo hints, *res;
    int fd, errcode;
    struct sockaddr_in addr;
    socklen_t addrlen;
    ssize_t n, nread; 
    char buffer[128];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket 
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    
    if((errcode = getaddrinfo(NULL, "58001", &hints, &res)) != 0) //error 
        exit(1);
    if((fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) ==-1 ) //error 
        exit(1);
    if(bind(fd, res->ai_addr, res->ai_addrlen) == -1) //error
        exit(1);
        
    while(1){ 
        addrlen = sizeof(addr);
        nread = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr,&addrlen);
        
        if(nread==-1) //error
            exit(1);
        n = sendto(fd, buffer, nread, 0, (struct sockaddr*)&addr,addrlen);
        
        if(n==-1) //error 
            exit(1);
    } 
    //freeaddrinfo(res); 
    //close(fd);
    //exit(0);
}