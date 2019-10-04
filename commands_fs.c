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
#include <signal.h>
#include "commands_fs.h"

int parse_command(char* message) {

    printf("Estou no commands_fs e recebi a mensagem: %s", message);

    int numTokens = 0;
    char *saveTokens[7];
    int numberChar;
   
    numberChar = strlen(message);
    message[strcspn(message, "\n")] = 0;; /*remove the \n added by fgets*/
    char *token = strtok(message, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    command_received(numTokens, saveTokens, message, numberChar);
    return 0;
}

int onlyNumbers(char* message) {
    int i;

    for(i = 0; i < strlen(message);i++){
        if(message[i] < 48 || message[i] > 57){
            return 0;
        }
    }
    return 1;
}

void command_received(int numTokens, char** saveTokens, char* input, long int numberChar){

    char message[1024];
    //comentarios do que e para fazer referem-se aos comandos mesmo em si

    if(commandREGOK(numTokens, saveTokens, numberChar)){
        sendCommandUDP("RGR OK\n");
        //function to execute the command RGR
    }else{
        sendCommandUDP("RGB NOK\n");
    }

    if(commandLTPOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LTR ");
        //function to execute the command LTR
        strcpy(message, "\n");
        sendCommandUDP(message);
    }

    if(commandPTPOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "PTR ");
        //Pode responder ok, dup ou ful
        strcpy(message, "\n");
        //function to execute the command PTR
        sendCommandUDP(message);
    }else{
        sendCommandUDP("PTR NOK\n");
    }

    if(commandLQROK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LQR ");
        //function to execute the command LQR
        sendCommandUDP(message);
        strcpy(message, "\n");
    }
    
    if(commandQGROK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QGR ");
        //function to execute the command QGR
        strcpy(message, "\n");
        sendCommandUDP(message);
    }else{
        sendCommandTCP("QGR EOF\n");
    }

    if(commandQUROK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QUR ");
        //function to execute the command QUR
        strcpy(message, "\n");
        sendCommandTCP(message);
    }else{
        sendCommandTCP("QUR NOK\n");
    }
    
    if(commandANROK(numTokens, saveTokens, numberChar)){
        strcpy(message, "ANR ");
        //function to execute the command ANR
        strcpy(message, "\n");
        sendCommandTCP(message);
    }
    
    //sendCommandUDP("ERR\n");

}
