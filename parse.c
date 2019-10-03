#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include "user.h"
#include "parse.h"

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


void input_action(int numTokens, char** saveTokens, char* input, long int numberCar) {
        
    if(commandREGOK(numTokens, saveTokens, numberCar)) {
        char message[45] = "REG";
        printf("register or reg\n");
        strcat(message, saveTokens[1]);
        strcat(message, "\n");
        sendCommandUDP(saveTokens[1]);
    }
     else if(!strcmp(saveTokens[0],"topic_list") || !strcmp(saveTokens[0], "tl")) {
        printf("topic list or tl\n");
        sendCommandUDP("TL\n");
    }
    else if(!strcmp(saveTokens[0], "topic_select")){
        printf("topic select\n");
        strcat(saveTokens[1], "\n");
        sendCommandUDP(saveTokens[1]);    
    }
    else if(!strcmp(saveTokens[0], "ts")){
        printf("ts\n");
        strcat(saveTokens[1], "\n");
        sendCommandUDP(saveTokens[1]);   
    }
    else if(!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp")){
        printf("topic propose or tp\n");
        strcat(saveTokens[1], "\n");
        sendCommandUDP(saveTokens[1]);
    }
    else if(!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql")){
        printf("question list or ql\n");
        sendCommandUDP("QL\n");
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
        sendCommandTCP();
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        printf("Invalid syntax!\n"); 
    }
}

int parse_input_action() {

    int numTokens = 0;
    char *saveTokens[7];
    char input[50];
    int numberCar;
    if(fgets(input, 50, stdin) == NULL){
        return -1;
    }    
    numberCar = strlen(input);
    input[strcspn(input, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(input, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action(numTokens, saveTokens, input, numberCar);
    return 0;
}