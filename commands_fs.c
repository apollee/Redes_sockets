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
    int numberCar;
    if(fgets(message, 50, stdin) == NULL){
        return -1;
    }    
    numberCar = strlen(message);
    message[strcspn(message, "\n")] = 0;; /*remove the \n added by fgets*/
    char *token = strtok(message, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    command_received(numTokens, saveTokens, message, numberCar);
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

void command_received(int numTokens, char** saveTokens, char* input, long int numberCar){

    char message[1024];
    printf("tou a bazar\n");
}
