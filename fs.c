#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define FLAG "flag"
#define DEFAULT_PORT "58041"

#define max(A, B) ((A)>=(B)?(A):(B))

/*------------------------*/

int createSocket(struct addrinfo* res);

/*------------------------*/

	struct addrinfo hintsUDP,*resUDP;
    struct addrinfo hintsTCP,*resTCP;
    int fdUDP, fdTCP, errcode, newfd;
    ssize_t n;
	struct sockaddr_in addr;  
    socklen_t addrlen;
    extern int errno;
    fd_set rfds;


void input_command_user(int argc, char *argv[], char *port, char *ip) {
    strcpy(port, DEFAULT_PORT);
    strcpy(ip, FLAG);

    if(argc == 1){
        return;
    }
    else if(argc == 3 && !(strcmp(argv[1],"-n"))) {
        strcpy(ip,argv[2]);
    }
    else if(argc == 3 && !(strcmp(argv[1],"-p"))) {
        strcpy(port,argv[2]);
    }
    else if(argc == 5 && !(strcmp(argv[1],"-n")) && !(strcmp(argv[3],"-p"))) {
        strcpy(ip, argv[2]);
        strcpy(port,argv[4]);
    }
    else{
        fprintf(stderr, "Invalid syntax!\n");
        exit(-1);
    }

}

/* =============================================================================
 * input_command_server - input server when starting the program
 * =============================================================================
 */

int input_command_server(int argc, char *argv[], char* port) {
    strcpy(port, DEFAULT_PORT);

    if(argc == 1) {
        return 0;
    }
    else if(argc == 3 && (strcmp(argv[1],"-p") == 0)) {
        strcpy(port, argv[2]);
        return 0;
    }
    else{
        printf("Invalid syntax.\n");
        return -1;
    }
}

/* =============================================================================
 * parse_input_action - parsing the command to perform
 * =============================================================================
*/


int main(int argc, char *argv[]) {
    char buffer[128];
    char port[6];
    int maxDescriptor, counter;
    

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
    if(setsockopt(fdTCP, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
    	printf("setsockopt(SO_REUSEADDR) failed");
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
    
  	while(1){
        FD_ZERO(&rfds);
        FD_SET(fdUDP, &rfds);
        FD_SET(fdTCP, &rfds);       

        maxDescriptor = max(fdTCP, fdUDP);

        counter = select(maxDescriptor + 1, &rfds, NULL, NULL, NULL);

        if(FD_ISSET(fdUDP, &rfds)){
           addrlen = sizeof(addr);
            n = recvfrom(fdUDP, buffer, 128, 0,(struct sockaddr*)&addr,&addrlen);
            printf("%s\n", buffer);
            n = sendto(fdUDP, buffer, n, 0, (struct sockaddr*)&addr, addrlen);
        }

        if(FD_ISSET(fdTCP, &rfds)){
            int newfd = accept(fdTCP, (struct sockaddr*)&addr, &addrlen);
            printf("%d", newfd);
            int b = read(newfd, buffer, 128);
            write(1, "received: \n", 11);
            write(1, buffer, b); //fs
            b = write(newfd, buffer, b); 
        }

    }
    freeaddrinfo(resUDP);
    close(fdUDP);

    freeaddrinfo(resTCP);
    close(newfd);
    close(fdTCP);
}


int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}

