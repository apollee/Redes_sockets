#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<netdb.h>
#include <string.h>

int main(void){
    struct addrinfo hints, *res;
    int fd, n;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP socket 
    hints.ai_flags = AI_NUMERICSERV;
    
    n = getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res);
    if(n != 0) //error
        exit(1);
    
    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(fd == -1) //error
        exit(1);
    
    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if(n == -1) //error
        exit(1);
    
    printf("/n");
}