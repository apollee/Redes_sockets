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

#define DEFAULT_PORT "58041"
#define FLAG "flag"

/*------------------------*/
void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip);
int createUDPSocket(struct addrinfo hints, struct addrinfo* res);
/*------------------------*/


int main(int argc, char *argv[]) {
    struct addrinfo hintsUDP, *resUDP;
    struct sockaddr_in addr;
    socklen_t addrlen;
    ssize_t n;
    char host_name[128];
    char port[6];
    char ip[INET_ADDRSTRLEN];

    gethostname(host_name,128);
    strcpy(port, FLAG);
    strcpy(ip, FLAG);
    input_command(argc, argv, port, ip);
   
    if(!strcmp(port, FLAG)){
    	strcpy(port, DEFAULT_PORT);
    }


    //getIp(hintsUDP, host_name, port, resUDP, ip);
    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;

    int errcode = getaddrinfo(host_name, port, &hintsUDP, &resUDP);
    
    if(!strcmp(ip, FLAG)){
        inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip);
    }

    printf("ip: %s\n", ip);
    printf("port: %s\n", port);
    printf("%s\n",host_name);
    
    int fd = createUDPSocket(hintsUDP, resUDP);

    sendto(fd,"Hello!\n",7,0,resUDP->ai_addr,resUDP->ai_addrlen);

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
    printf("ip: %s\n", ip);
    printf("port: %s\n", port);
    printf("%s\n",host_name);
}



int createUDPSocket(struct addrinfo hints, struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}