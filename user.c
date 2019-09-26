#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#define DEFAULT_PORT "58041"
#define FLAG "flag"

/*------------------------*/
void input_command(int argc, char *argv[], char *port, char *ip);
int parse_input_action();
void input_action(int numTokens, char** saveTokens);
void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip);
int createSocket(struct addrinfo hints, struct addrinfo* res);
/*------------------------*/


int main(int argc, char *argv[]) {
    struct addrinfo hintsUDP, *resUDP, hintsTCP, *resTCP;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char host_name[128], port[6], ip[INET_ADDRSTRLEN], buffer[128];
    int fd, n;

    gethostname(host_name,128);
    strcpy(port, FLAG);
    strcpy(ip, FLAG);

    input_command(argc, argv, port, ip);
   
    if(!strcmp(port, FLAG)){
    	strcpy(port, DEFAULT_PORT);
    }


    char* message = "Hello!\n";
    int message_len = strlen(message);


    //UDP-------------------------------------------------------

    getIp(hintsUDP, host_name, port, resUDP, ip);
   
    fd = createSocket(hintsUDP, resUDP);

    sendto(fd, message, message_len , 0, resUDP->ai_addr, resUDP->ai_addrlen);

    addrlen = sizeof(addr);
    recvfrom(fd, buffer, 128, 0,(struct sockaddr*)&addr,&addrlen);
    printf("%s\n", buffer);

    
    
    
    
    //TCP----------------------------------------
    
    memset(&hintsTCP, 0 ,sizeof hintsTCP);
    hintsTCP.ai_family = AF_INET;
    hintsTCP.ai_socktype = SOCK_STREAM; //TCP
    hintsTCP.ai_flags = AI_NUMERICSERV;
    
    getIp(hintsTCP, host_name, port, resTCP, ip);
    
    fd = createSocket(hintsTCP, resTCP);
    
    n = connect(fd, resTCP->ai_addr, resTCP->ai_addrlen);



    
    while(1){
        parse_input_action();
    }
}










void input_command(int argc, char *argv[], char *port, char *ip) {
    if(argc == 1){
        return;
    }
    else if(argc == 3 && !(strcmp(argv[1],"-n"))) {
        strcpy(ip,argv[2]);
    }
    else if(argc == 3 && !(strcmp(argv[1],"-p"))) {
        strcpy(port,argv[2]);
    }
    else if(argc == 5 && !(strcmp(argv[1],"-n") == 0) && !(strcmp(argv[3],"-p") == 0)) {
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

    if(!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg")) {
        printf("register or reg\n");
    }
    else if(!strcmp(saveTokens[0],"topic_list") || !strcmp(saveTokens[0], "tl")) {
        printf("topic list or tl\n");
    }
    else if(!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp")){
        printf("topic propose or tp\n");
    }
    else if(!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql")){
        printf("question list or ql\n");
    }
    else if(!strcmp(saveTokens[0], "question_submit") || !strcmp(saveTokens[0], "qs")){
        printf("question submit or qs\n");
    }
    else if(!strcmp(saveTokens[0], "answer_submit") || !strcmp(saveTokens[0], "as")){
        printf("answer submit\n");
    }
    else if(!strcmp(saveTokens[0], "topic_select")){
        printf("topic select\n");    
    }
    else if(!strcmp(saveTokens[0], "ts")){
            printf("ts\n");
    }
    else if(!strcmp(saveTokens[0], "question_get")){
        printf("question get\n");
    }
    else if(!strcmp(saveTokens[0], "qg")){
        printf("qg\n");
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        printf("Invalid syntax!"); 
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



int createSocket(struct addrinfo hints, struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}
