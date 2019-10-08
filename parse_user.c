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

void input_command_user(int argc, char *argv[], char *port, char *ip) {
    strcpy(port, DEFAULT_PORT);
    strcpy(ip, FLAG);
    strcpy(id_user, FLAG);
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

void input_action(int numTokens, char** saveTokens, char* input, long int numberChar){
    char message[1024]; 

    if((!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg")) && numTokens == 2) {
        if(commandREGOK(numTokens, saveTokens, numberChar)){
            strcpy(id_user, saveTokens[1]);
            strcpy(message, "REG ");
            strcat(message, saveTokens[1]);
            strcat(message, "\n");
            send_commandUDP(message);
        }
        else{
            printf("User %s not registered\n", saveTokens[1]); //nao devia ser um ERR tambem?
        }
    }
    else if((!strcmp(saveTokens[0], "topic_list") || !strcmp(saveTokens[0], "tl")) && numTokens == 1) {
        if(commandTLOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user)){
                printf("You need to register first\n");
            }
            else{
                strcpy(message, "LTP\n");
                send_commandUDP(message);
            }
        }
        else{
            printf("Failed to get the list of topics\n");
        }
    }
    else if((!strcmp(saveTokens[0], "topic_select") || !strcmp(saveTokens[0], "ts")) && numTokens == 2) {
        if(commandTSOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user)){
                printf("You need to register first\n");
            }
            else{
                printf("selected topic: %s\n", local_topic);
            }
        }else{
            printf("Failed to select the topic\n");
        }
    } 
    else if(commandTPOK(numTokens, saveTokens, numberChar)){
        if(!isREG(id_user)){
            printf("You need to register first\n");
        }
        else{
            strcpy(message, "PTP ");
            strcat(message, id_user);
            strcat(message, " ");
            strcat(message, saveTokens[1]);
            strcat(message, "\n");
            send_commandUDP(message);
        }
    }
    else if(commandQLOK(numTokens, saveTokens, numberChar)){
        if(!isREG(id_user)){
            printf("You need to register first\n");
        }
        else{
            strcpy(message, "LQU ");
            //strcat(message, saveTokens[1]);
            strcat(message, "\n");
            send_commandUDP(message);
        }
    }
    else if(commandQGOK(numTokens, saveTokens, numberChar)){
        if(!isREG(id_user)){
            printf("You need to register first\n");
        }
        else{
            strcpy(message, "GQU ");
            //adicionar o topico? nao sei de onde vem
            strcat(message, "\n");
            send_commandTCP(message);
        }
    }
    else if(commandQSOK(numTokens, saveTokens, numberChar)){
        if(!isREG(id_user)){
            printf("You need to register first\n");
        }
        else{
            strcpy(message, "QUS ");
            //strcat(message, ID(temos que adicionar isto))
            //adicionar o topico?? nao sei de onde vem?
            strcat(message, saveTokens[1]);
            //adicionar qsize, qdata, qimg
            strcat(message, "\n");
            send_commandTCP(message);
        }
    }
    else if(commandASOK(numTokens, saveTokens, numberChar)){
        if(!isREG(id_user)){
            printf("You need to register first\n");
        }
        else{
            strcpy(message, "ANS ");
            //strcat(message, ID(temos que adicionar isto))
            //adicionar o topico?? nao sei de onde vem?
            //adicionar a questao? nao sei de onde vem
            //adicionar asize, adata, aIMG
            strcat(message, "\n");
            send_commandUDP(message);
        }
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        strcpy(message, "ERR\n");
        send_commandUDP(message);
    }
}


int parse_command() {

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



void input_action_received(int numTokens, char** saveTokens, char* buffer, long int numberChar){
      //char message[1024]; 
    //Mudar esta funcao para fazer so uma chamada ao saveTokens[0]
    if((!strcmp(saveTokens[0],"RGR")) && numTokens == 2) {
        if(!strcmp(saveTokens[1], "OK")){
            printf("User \"%s\" registered\n", id_user);
        }
        else if(!strcmp(saveTokens[1], "NOK")){
            printf("User %s not registered\n", id_user); //e suposto?
        }
    }
    else if(!strcmp(saveTokens[0], "LTR")){ 
        printf("available topics:\n");
        topics_print(saveTokens);
    }/*
    else if(commandTSOK(numTokens, saveTokens, numberChar)){
        printf("topic select or ts\n");
        //only works localy   
    }*/
    else if(!strcmp(saveTokens[0], "PTR")){
       printf("%s", saveTokens[1]);
    }/*
    else if(commandQLOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LQU ");
        //strcat(message, saveTokens[1]);
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(commandQGOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "GQU ");
        //adicionar o topico? nao sei de onde vem
        strcat(message, "\n");
        send_commandTCP(message);
    }
    else if(commandQSOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QUS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        strcat(message, saveTokens[1]);
        //adicionar qsize, qdata, qimg
        strcat(message, "\n");
        send_commandTCP(message);
    }
    else if(commandASOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "ANS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        //adicionar a questao? nao sei de onde vem
        //adicionar asize, adata, aIMG
        strcat(message, "\n");
        send_commandUDP(message);
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        strcpy(message, "ERR\n");
        send_commandUDP(message);
    }*/
}

void parse_command_received(char* buffer){
    int numTokens = 0;
    char *saveTokens[120];
    int numberChar;
    
    numberChar = strlen(buffer);
    buffer[strcspn(buffer, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(buffer, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }

    input_action_received(numTokens, saveTokens, buffer, numberChar);
}

void topics_print(char** saveTokens){
    int i;
    int number = atoi(saveTokens[1]); 

    for(i = 1; i <= number; i++){
        printf("%d - ", i);
        char * token = strtok(saveTokens[i+1], ":");
        printf("%s ", token);
        token = strtok(NULL, ":");
        printf("(proposed by %s)\n", token);
    }
}