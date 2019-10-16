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
#include "fs.h"
#include "directory_structure_fs.h"

int commandREGOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)        
        return FALSE;
    else if(numberChar - 2 != strlen(saveTokens[0])+strlen(saveTokens[1]))        
        return FALSE;
    else if (strlen(saveTokens[1]) != 5)        
        return FALSE;
    else if(!onlyNumbers(saveTokens[1]))        
        return FALSE;
    else
        return TRUE;
}

int commandLTPOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 1){
        return FALSE; 
    }
    //     else if(numberChar != strlen(saveTokens[0])){
    //     printf("numberChar: %ld\n", numberChar);
    //     printf("strlen(saveTokens[0]): %ld\n", strlen(saveTokens[0]));
    //     return FALSE;
    //}
    else{
        return TRUE;
    }
}

int commandPTPOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 3)
        return FALSE;
    else if(numberChar - 3 != strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2])){
        return FALSE;
    }
    else 
        return TRUE;     
}

int commandLQUOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE; 
    /*else if(numberChar - 2 != strlen(saveTokens[0]) + strlen(saveTokens[1])){
        printf("%ld\n", numberChar);
        printf("%ld\n",strlen(saveTokens[0]));
        printf("%ld\n",strlen(saveTokens[1]));
        return FALSE;
    }*/
    else
        return TRUE; 
}

//--------------------------------------------------------------------
// TCP CMDS
//--------------------------------------------------------------------
int commandGQUOK(char** saveTokens, long int numberChar){
    /*if(numberChar - 3 != strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]))
        return FALSE;*/
    if(strlen(saveTokens[1])>10 || strlen(saveTokens[2])>10)
        return FALSE;
    else 
        return TRUE;
}

int commandQUSOK(char** saveTokens, int numberChar){
    if(!strlen(saveTokens[0]) || !strlen(saveTokens[1]) || !strlen(saveTokens[2]) || !strlen(saveTokens[3])|| !strlen(saveTokens[4]))
        return FALSE;
    else if(numberChar - 5 != (strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]) + strlen(saveTokens[3])+strlen(saveTokens[4])))
        return FALSE;
    else if(strlen(saveTokens[1]) != 5 || !onlyNumbers(saveTokens[1]))
        return FALSE;
    else if(strlen(saveTokens[2])>10 || strlen(saveTokens[3])>10)
        return FALSE;
    else if(!onlyNumbers(saveTokens[1]))
        return FALSE;
    else
        return TRUE;
}
int commandANSOK(char** saveTokens, long int numberChar){
    //Nao sei se nao temos que verificar os ficheiros 
    // if(numTokens != 3)
    //     return FALSE;
    // else if(numberChar - 3 != strlen(saveTokens[0])  + strlen(saveTokens[1]) + strlen(saveTokens[2]))
    //     return FALSE;
    // else 
        return TRUE;
}

//Outras funções
int onlyNumbers(char* message) {
    int i;
    for(i = 0; i < strlen(message);i++){
        if(message[i] < 48 || message[i] > 57){
            return FALSE;
        }
    }
    return TRUE;
}

//TODO: Podemos passar estas funcões para o parse---------------------
//TOPIC LIST
char* checkTopics(){
    char* message = (char*)malloc(sizeof (char)* 1024);
    memset(message, 0, 1024);
    char* finalMessage;
    char* numTopics;
    numTopics = number_of_topics();
    strcpy(message, numTopics);
    if (strcmp(numTopics, "0")){
        strcat(message, " ");
        strcat(message, topicList(numTopics));
    }
    finalMessage = (char*)realloc(message, strlen(message)+1);
    return finalMessage;
}

//TOPIC PROPOSE
char* proposeTopic(char** saveTokens){
    char* message = (char*)malloc(sizeof (char)* 1024);
    memset(message, 0, 1024);
    char* finalMessage;
    char* path = malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s", saveTokens[2]);
    
    if (check_directory_existence(path))
        strcpy(message, "DUP");
    else if (!check_max_directory_size(path))
        strcpy(message, "FUL");
    else {
        create_topic_directory(saveTokens[2], saveTokens[1]);
        strcpy(message, "OK");
    }
    //free(path);
    finalMessage = (char*)realloc(message, strlen(message)+1);
    return finalMessage;
}

char* checkSubmitQuestion(char** saveTokens){
    char* message = (char*)malloc(sizeof (char)* 1024);
    memset(message, 0, 1024);
    char* finalMessage;
    char* pathTopic = malloc(sizeof (char)* 1024);
    char* pathQuestion = malloc(sizeof (char)* 1024);
    sprintf(pathTopic, "TOPICS/%s", saveTokens[2]);
    sprintf(pathQuestion, "TOPICS/%s/%s", saveTokens[2], saveTokens[3]);

    if (!check_directory_existence(pathTopic)){
        strcpy(message, "NOK");
    }
    else if(check_directory_existence(pathQuestion))
        strcpy(message, "DUP");
    else if(!check_max_directory_size(pathTopic))
        strcpy(message, "FUL");
    else{
        strcpy(message, "OK");
        createQuestion(pathTopic, saveTokens);
    }
    
    //free(path);
    finalMessage = (char*)realloc(message, strlen(message)+1);
    return finalMessage;

}


//QUESTION LIST
char* checkQuestions(char** saveTokens){
    char* message = (char*)malloc(sizeof (char)* 1024);
    memset(message, 0, 1024);
    char* finalMessage;
    char* path = malloc(sizeof (char)* 1024);
    sprintf(path, "TOPICS/%s/", saveTokens[1]);
    char* numDirec = numberOfdirectories(path); 
    strcpy(message, numDirec);
    if(!strcmp(message, "0")){
        message = (char*)realloc(message, strlen(message)+1);
        return message;
    }
    strcat(message, questionList(saveTokens[1], numDirec));
    //free(path);
    finalMessage = (char*)realloc(message, strlen(message)+1);
    return finalMessage;
}