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
#include "parse_user.h"
#include "commands_user.h"
#include "directory_structure.h"


void input_command_user(int argc, char *argv[], char *port, char *ip) {

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

int parse_command() { // command that the user wrote

    int numTokens = 0;
    char *saveTokens[7];
    char input[50];
    int numberChar;
    if(fgets(input, 50, stdin) == NULL){
        return -1;
    }    

    numberChar = strlen(input);
    input[strcspn(input, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(input, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action(numTokens, saveTokens, input, numberChar);
    return 0;
}

//action that the user requested
void input_action(int numTokens, char** saveTokens, char* input, long int numberChar){ 
    char message[1024]; 

    //--------------------------------------------------------------------
    // UDP CMDS
    //--------------------------------------------------------------------
    if((!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg"))) {
        if(commandREGOK(numTokens, saveTokens, numberChar))
            send_message_reg(saveTokens[1], message);
        else
            printf("Invalid arguments\n"); //nao devia ser um ERR tambem?
    }

    else if((!strcmp(saveTokens[0], "topic_list") || !strcmp(saveTokens[0], "tl"))) {
        if(commandTLOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
            	printf("You should register first\n");
            else
                send_message_tl(message);
        }
        else
            printf("Invalid arguments\n"); 
    }

    else if((!strcmp(saveTokens[0], "topic_select") || !strcmp(saveTokens[0], "ts"))) {
        if(commandTSOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
            	printf("You should register first\n");
            else
            	printf("selected topic: %s (%s)\n", local_topic, topicID(local_topic));
        }else
            printf("Invalid arguments\n");
    } 

    else if((!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp"))) {
        if(commandTPOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else
                send_message_tp(saveTokens[1], message);
        }else{
            printf("Invalid arguments\n");
        }
    }

    else if((!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql"))){
        if(commandQLOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else if (!isREG(local_topic))
                printf("You have to select a topic first\n");
            else
                send_message_ql(message);
        }else{
            printf("Invalid arguments\n");
        }
    }
    //--------------------------------------------------------------------
    // TCP CMDS
    //--------------------------------------------------------------------
    else if((!strcmp(saveTokens[0], "question_get") || !strcmp(saveTokens[0], "qg"))){
        if(commandQGOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else
                send_message_qg(message);
        }else{
            printf("Invalid arguments\n");
        }
    }
    
    else if((!strcmp(saveTokens[0], "question_submit") || !strcmp(saveTokens[0], "qs"))){
        if(commandQSOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else{
                send_message_qs(message, numTokens, saveTokens);
            }
        }else{
            printf("Invalid arguments\n");
        }
    }

    else if((!strcmp(saveTokens[0], "answer_submit") || !strcmp(saveTokens[0], "as"))){
        if(commandASOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else
                send_message_as(message);
        }else{
            printf("Invalid arguments\n");
        }
    }

    else if(!strcmp(saveTokens[0], "exit"))
        exit(0);

    else
        //send_message_err(message);
       printf("Invalid command\n");
}

//comand that the user got from the server
void parse_command_received(char* buffer){ 
    int numTokens = 0;
    char *saveTokens[120];
    int numberChar;
    
    numberChar = strlen(buffer);
    buffer[strcspn(buffer, "\n")] = 0; 
    char *token = strtok(buffer, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }

    input_action_received(numTokens, saveTokens, buffer, numberChar);
}

void input_action_received(int numTokens, char** saveTokens, char* buffer, long int numberChar){ //message from the server
    char command[5];
    strcpy(command, saveTokens[0]);

    if((!strcmp(command,"RGR")) && numTokens == 2) {
        if(!strcmp(saveTokens[1], "OK")){
            printf("User \"%s\" registered\n", id_user);
        }
        else if(!strcmp(saveTokens[1], "NOK")){
            printf("User %s not registered\n", id_user); //e suposto?
        }
    }
    else if(!strcmp(command, "LTR")){ 
        printf("available topics:\n");
        topics_print(saveTokens);
    }
    else if(!strcmp(command, "PTR")){
       printf("%s %s\n", saveTokens[0], saveTokens[1]);
       
    }
    else if(!strcmp(command, "LQR")){
        printf("available questions about %s:\n", local_topic);
        questions_print(saveTokens);
    }
    /*else if(commandQGOK(numTokens, saveTokens, numberChar)){
            
    }*/
    else if(!strcmp(command, "QUR")){
        printf("%s %s\n", command, saveTokens[1]);    
    }/*
    else if(commandASOK(numTokens, saveTokens, numberChar)){
        
    }*/
    else{
        printf("Unexpected server response\n");
    }
}