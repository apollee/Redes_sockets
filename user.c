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
#include <sys/time.h>
#include "parse_user.h" 
#include "user.h"
#include "commands_user.h"

struct addrinfo hintsUDP, hintsTCP;
struct addrinfo *resUDP, *resTCP;
struct sockaddr_in addr;
socklen_t addrlen;
ssize_t n;
int fdUDP, errcode;
char host_name[128];
char port[6];
char ip[55];
 
int main(int argc, char *argv[]) {
    
    sigpipe_handler();
    initialize_flags();
    if(gethostname(host_name,128) == -1) {
        fprintf(stderr, "error: %s\n", strerror(errno));
    }

    input_command_user(argc, argv, port, ip); //check argv commands

    //UDP-------------------------------------------------------------------
    start_UDP();
    fdUDP = create_socket(resUDP);
    if(fdUDP == -1){
        printf("creating UDP socket failed\n");
    }

    //TCP-------------------------------------------------------------------
    start_TCP();
    fdTCP = create_socket(resTCP);
    if(fdTCP == -1){
        printf("creating TCP socket failed\n");
    }

    //start the program
    while(1){ 
        parse_command(); 
    }
    free_and_close(); 
}  

void sigpipe_handler(){
    struct sigaction act;    
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;

    if(sigaction(SIGPIPE,&act,NULL)== -1){
        printf("Error: connection was lost.\n");    
        exit(-1); 
    }  
}

void initialize_flags(){
    strcpy(port, DEFAULT_PORT);
    strcpy(ip, FLAG);
    strcpy(id_user, FLAG);
    strcpy(local_topic, FLAG);
}

int create_socket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(fd == -1){
        printf("Error: not able to communicate to the server.\n");
        exit(-1);
    }
    return fd;
}

void start_UDP(){
    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;
    
    if(!strcmp(ip, FLAG)){
        if((errcode = getaddrinfo(host_name, port, &hintsUDP, &resUDP)) != 0){
            printf("Error: receiving server information.\n");
            exit(-1);
        }
        if(inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip) == NULL){
            printf("Error: receiving server information.\n");
            exit(-1);
        }
    }
    else{
        if((errcode = getaddrinfo(ip, port, &hintsUDP, &resUDP)) != 0){
            printf("Error: receiving server information.\n");
            exit(-1);
        }
    }
}

void send_commandUDP(char *message){
    n = sendto(fdUDP, message, strlen(message),0,resUDP->ai_addr,resUDP->ai_addrlen);

    if(n == -1){
        printf("Error: not able to send the message to the server.\n");
        exit(-1);
    }
    char* buffer = (char*)malloc(sizeof( char)*2048);
    memset(buffer, 0, 2048);
    addrlen = sizeof(addr);

    
    struct timeval time;
    time.tv_sec = 10;
    time.tv_usec = 0;
    setsockopt(fdUDP, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time, sizeof time);
    
    n = recvfrom(fdUDP, buffer, 2048, 0, (struct sockaddr*) &addr, &addrlen);
    if(n == -1){
        printf("Error: not able to receive the message from the server.\n");
        exit(-1);
    }
    parse_command_received(buffer);
    free(buffer);
}

void start_TCP(){
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_NUMERICSERV;

    if(!strcmp(ip, FLAG)){
        if((errcode = getaddrinfo(host_name, port, &hintsTCP, &resTCP)) != 0){
            printf("Error: receiving server information.\n");
            exit(-1);
        }
    }
    else{
        if ((errcode = getaddrinfo(ip, port, &hintsTCP, &resTCP)) != 0){
            printf("Error: receiving server information.\n");
            exit(-1);
        }
    }
} 

int connectTCP(){
    int h = connect(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    if(h == -1){
        printf("Error: not able to connect to the server.\n");
        exit(-1);
    }  
    return h; 
}

int writeTCP(char* message, int nread){
    ssize_t n;
    if(nread != 0){
        n = write(fdTCP, message, nread); //images
        if (n == -1){
            printf("Error: not able to send the message to the server.\n");
            exit(-1);
        }
    }else{
        n = write(fdTCP, message, strlen(message)); //text
        if (n == -1){
            printf("Error: not able to send the message to the server.\n");
            exit(-1);
        }
    }   
    return n;
}

char* readTCP(){
    char* buffer = (char*) malloc(sizeof(char)*1024);
    char* bufferFinal;
    int b = read(fdTCP, buffer, 1024); 
    if (b == -1){
        printf("Error: not able to receive the message from the server.\n");
        exit(-1);
    }
    bufferFinal = realloc(buffer, strlen(buffer)+1);
    return bufferFinal;
}

void free_and_close(){
    freeaddrinfo(resUDP);
    freeaddrinfo(resTCP);
    close(fdUDP); 
    close(fdTCP);  
} 