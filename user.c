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
//char ip[INET_ADDRSTRLEN];
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
    if(sigaction(SIGPIPE,&act,NULL)==-1)/*error*/exit(1);   
}

void initialize_flags(){
    strcpy(port, DEFAULT_PORT);
    strcpy(ip, FLAG);
    strcpy(id_user, FLAG);
    strcpy(local_topic, FLAG);
}

int create_socket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}

void start_UDP(){
    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;
    
    if(!strcmp(ip, FLAG)){
        if((errcode = getaddrinfo(host_name, port, &hintsUDP, &resUDP)) != 0){
            fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
        }
        inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip);
    }
    else{
        if((errcode = getaddrinfo(ip, port, &hintsUDP, &resUDP)) != 0){
            fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
        }
    }
}

void start_TCP(){
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_NUMERICSERV;

    if(!strcmp(ip, FLAG)){
        if((errcode = getaddrinfo(host_name, port, &hintsTCP, &resTCP)) != 0){
            fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
        }
    }
    else{
        if ((errcode = getaddrinfo(ip, port, &hintsTCP, &resTCP)) != 0){
            fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
        }
    }
} 




//For sending UDP CMDS
void send_commandUDP(char *message){   
    n = sendto(fdUDP, message, strlen(message),0,resUDP->ai_addr,resUDP->ai_addrlen);
    if(n == -1){
        printf("send to not working UDP\n");
    }

    char* buffer = (char*)malloc(sizeof( char)*2048);
    memset(buffer, 0, 2048);
    addrlen = sizeof(addr);
    n = recvfrom(fdUDP, buffer, 2048, 0, (struct sockaddr*) &addr, &addrlen);
    if(n == -1){
        printf("receiving from UDP server not working\n");
    }
    parse_command_received(buffer);
    free(buffer);
}


//divididos as 3 funcoes porque nao queremos fazer connect varias vezes no ciclo while
int connectTCP(){
    int h = connect(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    if(h == -1){
        printf("send to not working TCP\n");
    }  
    return h; 
}

int writeTCP(char* message){
    ssize_t b = write(fdTCP, message, strlen(message));
    if (b == -1){
        return b;
    }
    return b;
}

char* readTCP(char* buffer){
    //char* bufferFinal;
    int b = read(fdTCP, buffer, 1024); 
    if (b == -1){
        printf("read not working TCP");
    }

    return buffer;
}

//Esta funcao e para ser chamada depois de tudo ter sido enviado
void send_commandTCP(char* message){ 
    //resTCP->ai_addrlen = sizeof(resTCP->ai_addr);
    int h = connect(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    if(h == -1){
        printf("connect not working TCP\n");
    }

    int b = send(fdTCP, message, strlen(message), 0);
    if (b == -1){
        printf("write not working TCP\n");
    }

    char* buffer = (char*)malloc(sizeof(char)*2048);
    memset(buffer, 0, 2048);
    strcpy(buffer, "");

    b = recv(fdTCP, buffer, 50, 0); 
    if (b == -1){
        perror("receive not working");
    }

    write(1, "echo TCP: ", 10);    
    write(1, buffer, strlen(buffer)); 
    parse_command_received_TCP(buffer);
    close(fdTCP);

    fdTCP = create_socket(resTCP);
    if(fdTCP == -1){
        printf("creating TCP socket failed\n"); 
    }
} 





void free_and_close(){
    freeaddrinfo(resUDP);
    freeaddrinfo(resTCP);
    close(fdUDP); 
    close(fdTCP);  
} 