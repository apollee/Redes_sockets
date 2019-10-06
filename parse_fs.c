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
#include "parse_fs.h"
#include "commands_fs.h"
#include "directory_structure.h"

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

char* input_action(int numTokens, char** saveTokens, char* input, long int numberChar){
    char *message = malloc (sizeof (char) * 1024);
    
    if(!strcmp(saveTokens[0], "REG")){
        if(commandREGOK(numTokens, saveTokens, numberChar)){
            strcpy(message, "RGR OK\n");
            printf("user: ip %s\n", saveTokens[1]);
            printf("directory structure: %d\n", ListDir("Lab1"));
        }
        else
            strcpy(message, "RGR NOR\n");
        return message;
    }

    else if(commandLTPOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LTR ");
        printf("List topics\n");
        //function to execute the command LTR
        strcpy(message, "\n");
        return message;
        //sendCommandUDP(message);
    }

    else if(commandPTPOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "PTR ");
        //Pode responder ok, dup ou ful
        strcpy(message, "\n");
        return message;
        //function to execute the command PTR
        //sendCommandUDP(message);
    }

    else if(commandLQUOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "LQR ");
        //function to execute the command LQR
        //sendCommandUDP(message);
        strcpy(message, "\n");
        return message;
    }
    
    else if(commandGQUOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QGR ");
        //function to execute the command QGR
        strcpy(message, "\n");
        return message;
        //sendCommandUDP(message);
    }

    else if(commandQUSOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "QUR ");
        //function to execute the command QUR
        strcpy(message, "\n");
        return message;
        //sendCommandTCP(message);
    }
    
    else if(commandANSOK(numTokens, saveTokens, numberChar)){
        strcpy(message, "ANR ");
        //function to execute the command ANR
        strcpy(message, "\n");
        return message;
        //sendCommandTCP(message);
    }
    else{
        //Porque é que aqui o \n esta no mesmo strcpy do ERR e não num a 
        //parte como no resto das funcoes
        strcpy(message, "ERR\n");
        return message;
    }
    
    //sendCommandUDP("ERR\n");
}

char* parse_command(char* message) { 

    int numTokens = 0;
    char *saveTokens[7];
    int numberChar;
   
    numberChar = strlen(message);
    message[strcspn(message, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(message, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    return input_action(numTokens, saveTokens, message, numberChar);
}