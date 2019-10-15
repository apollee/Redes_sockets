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
#include "vector.h"

Link headUser;

int input_command_server(int argc, char *argv[], char* port) {
    strcpy(port, DEFAULT_PORT);
    headUser = NULL;
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

char* parse_command(char* message, const char* ip) { // command that is received
    int numberChar, numTokens;
    char *saveTokens[7];
    numberChar = strlen(message);
    message[strcspn(message, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(message, " ");

    for (numTokens = 0; token != NULL; numTokens++){
        saveTokens[numTokens] = token;
        token = strtok(NULL, " ");
    }
    
    return input_action(numTokens, saveTokens, message, numberChar, ip);
}

//
char* input_action(int numTokens, char** saveTokens, char* input, long int numberChar, const char* ip){
    char* message = (char*)malloc (sizeof (char) * 1024);
    memset(message, 0, 1024);
    //--------------------------------------------------------------------
    // UDP CMDS
    //--------------------------------------------------------------------    
    
    if(!strcmp(saveTokens[0], "REG")){
        if(commandREGOK(numTokens, saveTokens, numberChar)){
            headUser = insertUser(headUser, atoi(saveTokens[1]), ip); //dados utilizador
            strcpy(message, "RGR OK\n");
            printf("user: %s %s\n", ip, saveTokens[1]);
        }
        else
            strcpy(message, "RGR NOK\n");
    }

    else if(!strcmp(saveTokens[0], "LTP")){
        if(commandLTPOK(numTokens, saveTokens, numberChar)){
            if(isREG(ip)){
            strcpy(message, "LTR ");
            printf("List topics\n");
            strcat(message, checkTopics());
            strcat(message, "\n");
        	}
        	else{
        		strcpy(message, "ERR\n");
        	}
        }
    }

    //Falta so verificacao de erros para quando nao foi dado REG
    else if(!strcmp(saveTokens[0], "PTP")){
        if(commandPTPOK(numTokens, saveTokens, numberChar)){
            if(isREG(ip)){
	            strcpy(message, "PTR ");
	            strcat(message, proposeTopic(saveTokens));
	            strcat(message, "\n");
	        }
	        else{
	        	strcpy(message, "ERR\n");
	        }
	    }    
        else{
            strcpy(message, "PTR NOK\n");
        }
    }

    else if(!strcmp(saveTokens[0], "LQU")){
        if(commandLQUOK(numTokens, saveTokens, numberChar)){
	        if(isREG(ip)){
	            printf("List questions for topic: %s\n", saveTokens[1]);
	            strcpy(message, "LQR ");
	            strcat(message, checkQuestions(saveTokens));
	            strcat(message, "\n");
    		}
    		else{
    			strcpy(message, "ERR\n");
    		}
        }else{
            strcpy(message, "ERR\n");
        }
    }

    //--------------------------------------------------------------------
    // TCP CMDS
    //--------------------------------------------------------------------
    else if(!strcmp(saveTokens[0], "GQU")){
        if(commandGQUOK(numTokens, saveTokens, numberChar)){
            if(isREG(ip)){
            	strcpy(message, "QGR ");
            	strcat(message, "\n");
        	}
        	else{
        		strcpy(message, "ERR\n");
        	}
        }else{
            strcpy(message, "ERR\n");
        }
    }

    else if(!strcmp(saveTokens[0], "QUS")){
        if(commandQUSOK(numTokens, saveTokens, numberChar)){ 
        	if(isREG(ip)){
            	strcpy(message, "QUR ");
            	strcat(message, checkSubmitQuestion(saveTokens));
            	strcat(message, "\n");
			}
			else{
				strcpy(message, "ERR\n");
			}        	
        }else{
            strcpy(message, "QUR NOK\n");
        }
    }
    
    else if(!strcmp(saveTokens[0], "ANS")){
        if(commandANSOK(numTokens, saveTokens, numberChar)){
            if(isREG(ip)){
            	strcpy(message, "ANR ");
            	strcat(message, "\n");
        	}
        	else{
        		strcpy(message, "ERR\n");
        	}
        }else{
            strcpy(message, "ERR\n");
        }
    }
    
    else{
        strcpy(message, "ERR\n");
    }
    char* finalMessage = realloc(message, strlen(message)+1);
    return finalMessage;
}

int isREG(const char* ip){
	return searchIP(headUser, ip);
}

char* parse_commandTCP(char* message, const char* ip){
    int i;
        
    char** saveTokens = (char **) malloc(sizeof (char*) * 5);
    
    for(i = 0; i < 5; i++){
        saveTokens[i] = (char *) malloc(sizeof(char)*50);
        memset(saveTokens[i], 0, 50);
    }
    
    int nSpaces = 0;
    int j = 0;
    int k = 0;
    int error = 0;

    int sizes[5] = {3,5,10,10,10}; 
    int nSizes = 0;

    //First part of parse until data
    for(i = 0; i < 50; i++){
        if(message[i] == ' '){
            if (strlen(saveTokens[j]) == 0){
                error = 1;
            }
            else{
                if (nSizes < 2){
                    if (strlen(saveTokens[nSizes]) != sizes[nSizes])
                        error = 1;
                }
                else {
                    if (strlen(saveTokens[nSizes]) > sizes[nSizes])
                        error = 1;
                }
            }

            nSizes++;
            nSpaces++;
            j++;
            k = 0;
        }
        else if(nSpaces == 5){
            break;
        }
        else{
            saveTokens[j][k] = message[i];
            k++;
        }
    }

    char* newMessage = (char*) malloc(sizeof(char)* 9);
    if (error){
        strcpy(newMessage, "ERR\n");
    }
    else {
        //char* data = (char*) malloc(sizeof(char)* saveTokens[4]);
        
        printf("Message: %s\n", message);


        if(!strcmp(saveTokens[0],"GQU")){
            printf("qg\n");
        }

        else if(!strcmp(saveTokens[0],"QUS")){
            //if(commandQUSOK(saveTokens, , i)){}

            strcpy(newMessage, "QUR OK\n");
        }
        else if(!strcmp(saveTokens[0],"ANS")){
            printf("answer_submit\n");
        }
        else{
            printf("lou e gay\n");
        }
    }

    free(saveTokens);
    char* finalMessage = realloc(newMessage, strlen(newMessage) + 1); 
    printf("Final Message: %s\n", finalMessage);
    return finalMessage;
}