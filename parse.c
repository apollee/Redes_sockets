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
#include "commands_user.h"

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
    
    char message[1024]; //AUMENTAR O BUFFER!!!!!

    if(commandREGOK(numTokens, saveTokens, numberCar)) {
        strcpy(message, "REG ");
        strcat(message, saveTokens[1]);
        strcat(message, "\n");
        sendCommandUDP(message);
    }
    else if(commandTLOK(numTokens, saveTokens, numberCar)) {
        sendCommandUDP("TLP \n");
    }
    else if(commandTSOK(numTokens, saveTokens, numberCar)){
        printf("topic select or ts\n");
        //only works localy   
    }
    else if(commandTPOK(numTokens, saveTokens, numberCar)){
        strcpy(message, "PTP ");
        //strcat(message, ID(temos que adicionar isto));
        strcat(message, " ");
        strcat(message, saveTokens[1]);
        strcat(message, "\n");
        sendCommandUDP(message);
    }
    else if(commandQLOK(numTokens, saveTokens, numberCar)){
        strcpy(message, "LQU ");
        //strcat(message, saveTokens[1]);
        strcat(message, "\n");
        sendCommandUDP(message);
    }
    else if(commandQGOK(numTokens, saveTokens, numberCar)){
        strcpy(message, "GQU ");
        //adicionar o topico? nao sei de onde vem
        strcat(message, "\n");
        sendCommandTCP(message);
    }
    else if(commandQSOK(numTokens, saveTokens, numberCar)){
        strcpy(message, "QUS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        strcat(message, saveTokens[1]);
        //adicionar qsize, qdata, qimg
        strcat(message, "\n");
        sendCommandTCP(message);
    }
    else if(commandASOK(numTokens, saveTokens, numberCar)){
        strcpy(message, "ANS ");
        //strcat(message, ID(temos que adicionar isto))
        //adicionar o topico?? nao sei de onde vem?
        //adicionar a questao? nao sei de onde vem
        //adicionar asize, adata, aIMG
        strcat(message, "\n");
        sendCommandUDP(message);
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        strcpy(message, "ERR\n");
        sendCommandUDP(message);
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
