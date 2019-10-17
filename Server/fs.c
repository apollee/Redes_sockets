#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include "commands_fs.h"
#include "parse_fs.h"
#include "fs.h"
#include "directory_structure_fs.h"
#include "vector.h"

struct addrinfo hintsUDP,*resUDP;
struct addrinfo hintsTCP,*resTCP;
struct sockaddr_in addr;
struct in_addr *addrUDP;
int errno;
int fdUDP, fdTCP, errcode, newfd, maxDescriptor;
socklen_t addrlen;
ssize_t n;
fd_set rfds;
char *buffer; 
char port[6]; 

int main(int argc, char *argv[]) {
    char bufferIP[INET_ADDRSTRLEN];
    int indice;
    buffer = (char*)malloc(sizeof(char)*1024);
    memset(buffer, 0, 1024);
    sigpipe_handler();
    if(!check_directory_existence("TOPICS")){
        create_directory("TOPICS");
    }
    input_command_server(argc, argv, port); //checks argv commands

    start_UDP();
    
    start_TCP();

    char* bufferBeforeParsing;
    char* bufferAfterParsing;
    char* bufferFinal;

    // char* bufferBeforeParsingTCP;
    // char* bufferAfterParsingTCP;
    // char* bufferFinalTCP;
    

  	while(1){
        FD_ZERO(&rfds); 
        FD_SET(fdUDP, &rfds);
        FD_SET(fdTCP, &rfds);       
 
        maxDescriptor = max(fdTCP, fdUDP);

        select(maxDescriptor + 1, &rfds, NULL, NULL, NULL);

        //UDP
        if(FD_ISSET(fdUDP, &rfds)){ 
            addrlen = sizeof(addr);
            n = recvfrom(fdUDP, buffer, 1024, 0,(struct sockaddr*)&addr,&addrlen);
            
            bufferBeforeParsing = realloc(buffer, n+1);
            bufferAfterParsing = (char*)malloc(sizeof(char)*1024);

            bufferAfterParsing =  parse_command(bufferBeforeParsing, inet_ntop(resUDP->ai_family,&addr,bufferIP,sizeof bufferIP)); 

            bufferFinal = realloc(bufferAfterParsing, strlen(bufferAfterParsing) + 1);  
            sendto(fdUDP, bufferFinal, strlen(bufferFinal), 0, (struct sockaddr*)&addr, addrlen);
        } 	

        //TCP
        if(FD_ISSET(fdTCP, &rfds)){   
            addrlen = sizeof(addr);
            int newfd = accept(fdTCP, (struct sockaddr*)&addr, &addrlen);
            buffer = (char*)malloc(sizeof(char)* 1024);
            char* message = (char*)malloc(sizeof(char)*20);
            memset(buffer, 0, 1024);
            read(newfd, buffer, 1024);
            char** saveTokens = parse_commandTCP(buffer);
            if(!strcmp(saveTokens[0],"GQU")){
                if(commandGQUOK(saveTokens, atoi(saveTokens[3]))){
                    parseQGU(saveTokens, newfd, buffer);

                    //strcpy(message,"QGR 12345 3 ola 0 0\n");
                    
                }
                else{
                    strcpy(message,"GQU NOK\n");
                }
            }

            else if(!strcmp(saveTokens[0],"QUS")){
                int num = atoi(saveTokens[4]);
                int i = atoi(saveTokens[5]);
                if(commandQUSOK(saveTokens, i)){
                    char* path = (char*)malloc(sizeof(char)*50);
                    memset(path,0,50);
                    strcpy(path, "TOPICS/");
                    strcat(path,saveTokens[2]);
                    strcat(path, "/");
                    strcat(path, saveTokens[3]);
                    if(check_directory_existence(path)){
                        strcpy(message, "QUR DUP\n");
                    }
                    else if(atoi(numberOfdirectories(path))==99){
                        strcpy(message, "DUP FUL\n");
                    }
                    else{
                        createQuestion(path, saveTokens);
                        while(num > 0){
                            indice = treatBufferData(saveTokens, i, num, buffer);
                            num = num - (indice - i);
                            i = 0;

                            if(indice == strlen(buffer)){
                                memset(buffer, 0, 1024);
                                n = read(newfd, buffer, 1024);
                                indice = 0;
                            }
                        } 
                        if(buffer[indice] == ' '){
                            indice++;
                            if(buffer[indice] == '0'){
                                strcpy(message, "QUR OK\n");
                            }
                            else{
                                indice+=2;
                                char* ext = (char*)malloc(sizeof(char)*3);
                                int j;
                                for(j = 0; j<3; j++, indice++){
                                    ext[j] = buffer[indice];
                                }
                                ext[j] = '\0';
                                indice ++;
                                char* isize = (char*)malloc(sizeof(char)*10);
                                memset(isize, 0, 10);
                                int indImg = 0;  
                                while(buffer[indice] != ' '){
                                    isize[indImg] = buffer[indice];
                                    indice++;
                                    indImg++;
                                }
                                isize[indImg] = '\0';
                                indice++;
                                i = indice;
                                int numImg = atoi(isize);
                                while(numImg > 0){
                                    indice = treatBufferImg(saveTokens, i, numImg, n, buffer, ext);
                                    numImg = numImg - (indice - i);
                                    i = 0;
                                    if(indice == n){
                                        memset(buffer, 0, 1024);
                                        n = read(newfd, buffer, 1024);
                                    }
                                }
                             strcpy(message, "QUR OK\n");
                             printf("DATA SEND\n");
                            }
                        }
                    }
                    
                }
                else{
                        strcpy(message, "QUR NOK\n");
                }  
                 char* newMessage = realloc(message, strlen(message)+1);
                write(newfd, newMessage, strlen(newMessage)); //ta a escrever para o user?
                close(newfd); 
                close(fdTCP);

                start_TCP();

            }

            else if(!strcmp(saveTokens[0],"ANS")){
                if(commandANSOK(saveTokens, atoi(saveTokens[5]))){
                    message = parseANS(saveTokens, newfd, n, buffer, message);   
                }
                else{
                    strcpy(message, "QUR NOK\n");
                }
                     char* newMessage = realloc(message, strlen(message)+1);
                    write(newfd, newMessage, strlen(newMessage)); //ta a escrever para o user?
                    close(newfd); 
                    close(fdTCP);

                    start_TCP(); 
            }

            else{
                strcpy(message, "ERR");
                char* newMessage = realloc(message, strlen(message)+1);
                write(newfd, newMessage, strlen(newMessage)); //ta a escrever para o user?
                close(newfd); 
                close(fdTCP);

                start_TCP();
            }

           
        }
    }
   
    freeaddrinfo(resUDP);
    freeaddrinfo(resTCP);
    close(fdUDP);   
}

int create_socket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}

void sigpipe_handler(){
    struct sigaction act;	
	memset(&act,0,sizeof act);
	act.sa_handler=SIG_IGN;	
	if(sigaction(SIGPIPE,&act,NULL)==-1)
        exit(1);
}

void start_UDP(){
    memset(&hintsUDP,0,sizeof hintsUDP);
    hintsUDP.ai_family=AF_INET;//IPv4
    hintsUDP.ai_socktype=SOCK_DGRAM;//UDP socket
    hintsUDP.ai_flags=AI_PASSIVE|AI_NUMERICSERV;

    if ((errcode = getaddrinfo(NULL, port, &hintsUDP, &resUDP)) != 0){
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    }
    fdUDP = create_socket(resUDP);
    if(fdUDP == -1){
        printf("creating Server UDP socket failed\n"); 
    }

    n = bind(fdUDP,resUDP->ai_addr,resUDP->ai_addrlen);
    if(n == -1){   
        printf("bind not working Server UDP\n");
    }
}
 
void start_TCP(){
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET; 
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_PASSIVE|AI_NUMERICSERV;

    if((errcode = getaddrinfo(NULL, port, &hintsTCP, &resTCP)) != 0){
        fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
    }

    fdTCP = create_socket(resTCP); 
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
    
}        