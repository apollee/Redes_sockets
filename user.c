#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "58041"
/*------------------------*/

void input_command(int argc, char *argv[], char *port, char *ip);
int parse_input_action();
void input_action(int numTokens, char** saveTokens);

/*------------------------*/


int main(int argc, char *argv[]) {
    struct addrinfo hintsUDP, *resUDP;
    struct sockaddr_in addr;
    int fd, errcode;
    socklen_t addrlen;
    ssize_t n;

    char host_name[128];
    gethostname(host_name,128);
    char port[8];
    char ip[INET_ADDRSTRLEN];

    strcpy(port,"flag");
    strcpy(ip,"flag");

    input_command(argc, argv, port, ip);
   
    if(!strcmp(port, "flag")){
    	strcpy(port, PORT);
    }

    memset(&hintsUDP, 0 ,sizeof hintsUDP);
    hintsUDP.ai_family = AF_INET;
    hintsUDP.ai_socktype = SOCK_DGRAM; //UDP
    hintsUDP.ai_flags = AI_NUMERICSERV;

    errcode = getaddrinfo(host_name, port, &hintsUDP, &resUDP);
    
    if(!strcmp(ip, "flag")){
        printf("ola\n");
        inet_ntop(resUDP->ai_family, &((struct sockaddr_in*)resUDP->ai_addr)->sin_addr, ip, sizeof ip);
    }
    printf("ip: %s\n", ip);
    printf("port: %s\n", port);
    printf("%s\n",host_name);
    
    while(1){
        parse_input_action();
    }
}

void input_command(int argc, char *argv[], char *port, char *ip) {
    if(argc == 1){
        return;
    }
    else if(argc == 3 && (strcmp(argv[1],"-n") == 0)) {
        strcpy(ip,argv[2]);
    }
    else if(argc == 3 && (strcmp(argv[1],"-p") == 0)) {
        strcpy(port,argv[2]);
    }
    else if(argc == 5 && (strcmp(argv[1],"-n") == 0) && (strcmp(argv[3],"-p") == 0)) {
        strcpy(ip, argv[2]);
        strcpy(port,argv[4]);
    }
    else{
        fprintf(stderr, "Invalid syntax!\n");
        exit(-1);
    }
}


int parse_input_action() {
    int numTokens = 0;
    char *saveTokens[7];
    char input[50];
    
    if(fgets(input, 50, stdin) == NULL){
        return -1;
    }    
    
    input[strcspn(input, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(input, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action(numTokens, saveTokens);
}

void input_action(int numTokens, char** saveTokens) {

    if((strcmp(saveTokens[0], "register") == 0) || (strcmp(saveTokens[0],"reg") == 0)) {
        printf("register or reg\n");
    }
    else if((strcmp(saveTokens[0],"topic_list") == 0) || (strcmp(saveTokens[0], "tl") == 0)) {
        printf("topic list or tl\n");
    }
    else if((strcmp(saveTokens[0], "topic_propose") == 0) || (strcmp(saveTokens[0], "tp") == 0)){
        printf("topic propose or tp\n");
    }
    else if((strcmp(saveTokens[0], "question_list") == 0) || (strcmp(saveTokens[0], "ql") == 0)){
        printf("question list or ql\n");
    }
    else if((strcmp(saveTokens[0], "question_submit") == 0) || (strcmp(saveTokens[0], "qs") == 0)){
        printf("question submit or qs\n");
    }
    else if((strcmp(saveTokens[0], "answer_submit") == 0) || (strcmp(saveTokens[0], "as") == 0)){
        printf("answer submit\n");
    }
    else if(strcmp(saveTokens[0], "topic_select") == 0){
        printf("topic select\n");    
    }
    else if(strcmp(saveTokens[0], "ts") == 0){
            printf("ts\n");
    }
    else if(strcmp(saveTokens[0], "question_get") == 0){
        printf("question get\n");
    }
    else if(strcmp(saveTokens[0], "qg") == 0){
        printf("qg\n");
    }
    else if(strcmp(saveTokens[0], "exit") == 0){
        exit(0);
    }
    else{
        printf("Invalid syntax!"); 
    }
}
