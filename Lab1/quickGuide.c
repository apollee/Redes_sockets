//test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

extern int errno;

int main(void){
    char buffer[128];

    //gethostname
    printf("---gethostname:\n");
    if(gethostname(buffer,128)==-1)
        fprintf(stderr,"error: %s\n",strerror(errno));
    else 
        printf("host name: %s\n", buffer);
    printf("\n");


    //getaddrinfo
    printf("---getaddrinfo:\n");
    struct addrinfo hints, *res, *p;
    int errcode;
    char buffer2[INET_ADDRSTRLEN];
    struct in_addr *addr;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    if((errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", NULL, &hints, &res)) != 0)
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    else{
        printf("canonical hostname: %s\n", res->ai_canonname);
        for (p = res; p != NULL; p = p->ai_next){
            addr = &((struct sockaddr_in *)p->ai_addr)->sin_addr;
            printf("internet address: %s (%08lX)\n", 
            inet_ntop(p->ai_family, addr, buffer2, sizeof buffer2), 
            (long unsigned int)ntohl(addr->s_addr));
        }
    freeaddrinfo(res);
    }
    printf("\n");


    //UDP, socket and sendto
    printf("---UDP, socket and sendto:\n");
    int fd;
    ssize_t n;
 /*   
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket 
    hints.ai_flags = AI_NUMERICSERV;
    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt","58001",&hints,&res);
    
    if(errcode!=0) //error
        exit(1);

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(fd==-1) //error 
        exit(1);
    
    n=sendto(fd, "Hello!\n", 7, 0, res->ai_addr, res->ai_addrlen);
    
    if(n==-1) //error
        exit(1);
    printf("Number of characters sent: %ld\n", n);
    freeaddrinfo(res);
    printf("\n");
*/

    //UDP and recvfrom
    printf("---UDP and recvfrom:\n");
    struct sockaddr_in addr2;
    socklen_t addr2len;
    char buffer3[128];

    addr2len = sizeof(addr2);
    n = recvfrom(fd,buffer, 128, 0, (struct sockaddr*)&addr2, &addr2len);
    
    printf("antes\n");
    if(n==-1){ //error
        fprintf(stderr,"error: %s\n",strerror(errno));
        exit(1);
    }
    printf("depois\n");
    
    write(1, "echo: ", 6); //stdout
    write(1,buffer3, n);
    close(fd);
    printf("\n");


    //getnameinfo
    printf("---getnameinfo:\n");
    struct sockaddr_in addr3;
    socklen_t addr3len;
    char buffer4[128];
    char host[NI_MAXHOST], service[NI_MAXSERV]; //consts in <netdb.h>
    
    addr3len=sizeof(addr3);
    n = recvfrom(fd, buffer4, 128, 0, (struct sockaddr*)&addr3, &addr3len);

    if(n==-1) //error
        exit(1);
    
    if((errcode = getnameinfo((struct sockaddr *)
    &addr3, addr3len, host, sizeof host, service, sizeof service, 0)) != 0)
        fprintf(stderr, "error: getnameinfo: %s\n", gai_strerror(errcode));
    else
        printf("sent by [%s:%s]\n", host,service);
    printf("\n");

    exit(0);
    
}