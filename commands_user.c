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
#include "commands_user.h"
#include "directory_structure.h"

int commandREGOK(int numTokens, char** saveTokens, long int numberChar){
    if (strlen(saveTokens[1]) != 5)        
        return FALSE;
    else if(!onlyNumbers(saveTokens[1]))        
        return FALSE;
    else if(numberChar - 2 != strlen(saveTokens[0])+strlen(saveTokens[1]))        
        return FALSE;       
    else
        return TRUE;
}

int commandTLOK(int numTokens, char** saveTokens, long int numberChar){
    if(numberChar - 1 != strlen(saveTokens[0]))
        return FALSE;
    else
        return TRUE;
}

int commandTSOK(int numTokens, char** saveTokens, long int numberChar){
    if(numberChar - 2 != strlen(saveTokens[0]) + strlen(saveTokens[1]))
        return FALSE;
    else if(!strcmp(saveTokens[0], "ts")){
        if(onlyNumbers(saveTokens[1])){
            if(getTopic_by_number(atoi(saveTokens[1])))
                return TRUE;
            else
                return FALSE;
        }else{
            return FALSE;
        }
    }
    else if(!strcmp(saveTokens[0], "topic_select")){
        if(checkExistenceofTopic(saveTokens[1])){
            strcpy(local_topic, saveTokens[1]);
            return TRUE;   
        }else 
            return FALSE;
    }
    else
        return FALSE;
}

int commandTPOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
    else if(numberChar - 2 != strlen(saveTokens[0]) + strlen(saveTokens[1]))
        return FALSE;
    else if(!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp"))
        return TRUE;
    else 
        return FALSE;     
}

int commandQLOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 1)
        return FALSE; 
    else if(numberChar - 1 != strlen(saveTokens[0]))
        return FALSE;
    else if(!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql"))
        return TRUE;
    else
        return FALSE; 
}

int commandQGOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
    else if(numberChar - 2 != strlen(saveTokens[0]) + strlen(saveTokens[1]))
        return FALSE;
    else if(!strcmp(saveTokens[0], "qg")){
        if(onlyNumbers(saveTokens[1])){
            return TRUE;
        }else{
            return FALSE;
        }
    }
    else if(!strcmp(saveTokens[0], "question_get"))
        return TRUE;
    else 
        return FALSE;
    
}

int commandQSOK(int numTokens, char** saveTokens, long int numberChar){
    //Nao sei se nao temos que verificar os ficheiros 
    if(numTokens != 4)
        return FALSE;
    else if(numberChar - 4 != (strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]) + strlen(saveTokens[3]))){
        return FALSE;
    }
    else if(!strcmp(saveTokens[0], "question_submit") || !strcmp(saveTokens[0], "qs"))
        return TRUE;
    else 
        return FALSE;
}

int commandASOK(int numTokens, char** saveTokens, long int numberChar){
    //Nao sei se nao temos que verificar os ficheiros 
    if(numTokens != 3)
        return FALSE;
    else if(numberChar - 3 != strlen(saveTokens[0])  + strlen(saveTokens[1]) + strlen(saveTokens[2]))
        return FALSE;
    else if(!strcmp(saveTokens[0], "answer_submit") || !strcmp(saveTokens[0], "as"))
        return TRUE;
    else 
        return FALSE;
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

int isREG(char* id_user){
    if(strcmp(id_user, "flag")){
        return TRUE;
    }
    else{
        return FALSE;
    }
}