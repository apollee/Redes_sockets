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

#define FLAG "flag"
#define DEFAULT_PORT "58041"
/*------------------------*/

void getIp(struct addrinfo hintsUDP, char *host_name, char *port, struct addrinfo *resUDP, char *ip);
int createSocket(struct addrinfo* res);

/*------------------------*/

   struct addrinfo hintsUDP, hintsTCP;
    struct addrinfo *resUDP, *resTCP;
    struct sockaddr_in addr;
    socklen_t addrlen;
    ssize_t n;
    int fdUDP, fdTCP, errcode;

void sendREG(char *message){
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

void sendQG(){
    char buffer[128];

    int h = connect(fdTCP, resTCP->ai_addr, resTCP->ai_addrlen);
    if(h == -1){
        printf("send to not working TCP\n");
    }   
    int b = write(fdTCP, "ola\n", 4);
    if (b == -1){
        printf("write not working TCP");
    }

    b = read(fdTCP, buffer, 128);
    if (b == -1){
        printf("read not working TCP");
    }

    write(1, "echo TCP: ", 10);
    write(1, buffer, b);

}

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
 * input_action - executes the command if valid
 * =============================================================================
 */
void input_action(int numTokens, char** saveTokens, char* input) {
        
    if((!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg")) && numTokens == 2) {
        printf("register or reg\n");
        strcat(saveTokens[1], "\n");
        sendREG(saveTokens[1]);
    }
     else if(!strcmp(saveTokens[0],"topic_list") || !strcmp(saveTokens[0], "tl")) {
        printf("topic list or tl\n");
        sendREG("TL\n");
    }
    else if(!strcmp(saveTokens[0], "topic_select")){
        printf("topic select\n");
        strcat(saveTokens[1], "\n");
        sendREG(saveTokens[1]);    
    }
    else if(!strcmp(saveTokens[0], "ts")){
        printf("ts\n");
        strcat(saveTokens[1], "\n");
        sendREG(saveTokens[1]);   
    }
    else if(!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp")){
        printf("topic propose or tp\n");
        strcat(saveTokens[1], "\n");
        sendREG(saveTokens[1]);
    }
    else if(!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql")){
        printf("question list or ql\n");
        sendREG("QL\n");
    }
    else if(!strcmp(saveTokens[0], "question_submit") || !strcmp(saveTokens[0], "qs")){
        printf("question submit or qs\n");
    }
    else if(!strcmp(saveTokens[0], "answer_submit") || !strcmp(saveTokens[0], "as")){
        printf("answer submit\n");
    }
    else if(!strcmp(saveTokens[0], "question_get")){
        printf("question get\n");
    }
    else if(!strcmp(saveTokens[0], "qg")){
        printf("qg\n");
        sendQG();
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        printf("Invalid syntax!"); 
    }
}

/* =============================================================================
 * parse_input_action - parsing the command to perform
 * =============================================================================
 */
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
    input_action(numTokens, saveTokens, input);
    return 0;
}

int main(int argc, char *argv[]) {
    char host_name[128];
    char port[6];
    char ip[INET_ADDRSTRLEN];

    if(gethostname(host_name,128) == -1) {
        fprintf(stderr, "error: %s\n", strerror(errno));
    }

    input_command_user(argc, argv, port, ip);

    //UDP
    //getIp(hintsUDP, host_name, port, resUDP, ip);
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
}


int createSocket(struct addrinfo* res){ 
    int fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    return fd;
}

