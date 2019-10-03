#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include "parse.h"
#include "user.h"
#include "commands_user.h"
 
#define FLAG "flag"
#define DEFAULT_PORT "58041"

struct addrinfo hintsUDP, hintsTCP;
struct addrinfo *resUDP, *resTCP;
struct sockaddr_in addr;
socklen_t addrlen;
ssize_t n;
int fdUDP, fdTCP, errcode;

int main(int argc, char *argv[]) {
    char host_name[128];
    char port[6];
    char ip[INET_ADDRSTRLEN];
    
    struct sigaction act;
    
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&act,NULL)==-1)/*error*/exit(1);   

    if(gethostname(host_name,128) == -1) {
        fprintf(stderr, "error: %s\n", strerror(errno));
    }

    input_command_user(argc, argv, port, ip);

    //UDP
    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;
  
    if((errcode = getaddrinfo(host_name, port, &hintsUDP, &resUDP)) != 0){
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    }
    
    if(!strcmp(ip, FLAG)){
        inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip);
    }
    
    fdUDP = createSocket(resUDP);
    if(fdUDP == -1){
        printf("creating UDP socket failed\n");
    }

    //TCP-------------------------------------------------------------------
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_NUMERICSERV;

    if ((errcode = getaddrinfo(NULL, port, &hintsTCP, &resTCP)) != 0){
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    }

    fdTCP = createSocket(resTCP);
    if(fdTCP == -1){
        printf("creating TCP socket failed\n");
    }

    /*if(setsockopt(fdTCP, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
        printf("setsockopt(SO_REUSEADDR) failed");
    }*/
    
    while(1){ 
        parse_input_action();
    }

    freeaddrinfo(resUDP);
    close(fdUDP);

    freeaddrinfo(resTCP);
    close(fdTCP);
}

void sendCommandUDP(char *message){
    char buffer[128];
    n = sendto(fdUDP, message, strlen(message) + 1,0,resUDP->ai_addr,resUDP->ai_addrlen);

    if(n == -1){
        printf("send to not working UDP\n");
    }

    addrlen = sizeof(addr);
    n = recvfrom(fdUDP, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
    if(n == -1){
        printf("receiving from UDP server not working\n");
    }
    write(1, "echo UDP: ", 10);
    write(1, buffer, n);
}

void sendCommandTCP(char* message){
    char buffer[128];

    int h = connect(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    if(h == -1){
        printf("send to not working TCP\n");
    } 

    int b = write(fdTCP, message, strlen(message));
    if (b == -1){
        printf("write not working TCP");
    }

    b = read(fdTCP, buffer, 128);
    if (b == -1){
        printf("read not working TCP");
    }

    write(1, "echo TCP: ", 10);
    write(1, buffer, b);
    close(fdTCP);

    fdTCP = createSocket(resTCP);
    if(fdTCP == -1){
        printf("creating TCP socket failed\n");
    }
}

int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}