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
#include <sys/stat.h>
#include <signal.h>
#include "commands_user.h"
#include "directory_structure_user.h"
#include "user.h"

struct stat qsize, isize;

int commandREGOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
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
    if(numTokens != 1)
        return FALSE;
    if(numberChar - 1 != strlen(saveTokens[0]))
        return FALSE;
    else
        return TRUE;
}

int commandTSOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
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
        }
        else{ 
            return FALSE;
        }
    }
    else{
        return FALSE;
    }
}

int commandTPOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
    else if(numberChar - numTokens != strlen(saveTokens[0]) + strlen(saveTokens[1]))
        return FALSE;
    else 
        return TRUE;     
}

int commandQLOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 1)
        return FALSE; 
    else if(numberChar - numTokens != strlen(saveTokens[0]))
        return FALSE;
    else
        return TRUE; 
}

int commandQGOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
    else if(numberChar - numTokens != strlen(saveTokens[0]) + strlen(saveTokens[1]))
        return FALSE;
    else if(!strcmp(saveTokens[0], "qg")){
        if(onlyNumbers(saveTokens[1])){
            if(getQuestion_by_number(atoi(saveTokens[1])))
                return TRUE;
            else
                return FALSE;
        }else{
            return FALSE;
        }
    }
    else if(!strcmp(saveTokens[0], "question_get")){
       if(checkExistenceofQuestion(saveTokens[1])){
            strcpy(local_question, saveTokens[1]);
            return TRUE;
        }else{
            return FALSE;
        }
    }else 
        return FALSE;
    
}

int commandQSOK(int numTokens, char** saveTokens, long int numberChar){
    //Nao sei se nao temos que verificar os ficheiros 
    if(numTokens < 3 || numTokens > 4){
        return FALSE;
    }
    else if (numTokens == 3 && numberChar - numTokens != (strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]))){
        return FALSE;
    }
    else if (numTokens == 4 && numberChar - numTokens != (strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]) + strlen(saveTokens[3]))){
        return FALSE;
    }
    else 
        return TRUE;
}

int commandASOK(int numTokens, char** saveTokens, long int numberChar){
    //Nao sei se nao temos que verificar os ficheiros 
    if(numTokens < 2 || numTokens > 3){
        return FALSE;
    }
    else if (numTokens == 2 && numberChar - numTokens != (strlen(saveTokens[0]) + strlen(saveTokens[1]))){
        return FALSE;
    }
    else if (numTokens == 3 && numberChar - numTokens != (strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]))){
        return FALSE;
    }
    else 
        return TRUE;
}

void send_message_reg(char* id, char* message){
    strcpy(id_user, id);
    strcpy(message, "REG ");
    strcat(message, id);
    strcat(message, "\n");
    send_commandUDP(message);
}

void send_message_tl(char* message){
    strcpy(message, "LTP\n");
    send_commandUDP(message);
}

void send_message_tp(char* topic, char* message){
    strcpy(message, "PTP ");
    strcat(message, id_user);
    strcat(message, " ");
    strcat(message, topic);
    strcat(message, "\n");
    send_commandUDP(message);
}

void send_message_ql(char* message){
    strcpy(message, "LQU ");
    strcat(message, local_topic);
    strcat(message, "\n");
    send_commandUDP(message);
}

void send_message_qg(char* message){
    strcpy(message, "GQU ");
    strcat(message, local_topic);
    strcat(message, " ");
    strcat(message, local_question);
    strcat(message, "\n");
    send_commandTCP(message);
}

// void send_message_qs(char* message, int numTokens, char** saveTokens){
//     char var[1024]; 
//     sprintf(message, "QUS %s %s %s ", id_user, local_topic, saveTokens[1]);
//     stat(saveTokens[2], &qsize);
//     sprintf(var, "%ld", qsize.st_size); //Size of text doc
//     strcat(message, var);
//     strcat(message, " ");
//     //Doc itself
//     //strcat(message, " ");
//     if (numTokens == 4){
//         strcat(message, "1 ");
//         //Missing extension of image
//         //strcat(message, " ");
//         stat(saveTokens[3], &isize);
//         sprintf(var, "%ld", isize.st_size); //Size of image
//         strcat(message, var);
//         //Image itself
//     }else {
//         strcat(message, "0");
//     }
//     strcat(message, "\n\0"); //atencao ao \0
//     send_commandTCP(message);
// }

void send_message_qs(char* message, int numTokens, char** saveTokens){
    long var;
    char buffer[1024];
    memset(buffer, 0, 1024);
    strcpy(buffer, "");
    char* file_with_extension = (char*)malloc(sizeof(char)*1024);
    memset(file_with_extension, 0, 1024);
    //char buffer[DEFAULT_BUFFER_SIZE];
    //int offset = 0;
    int indice = 0;
    int numBytes; 
    //printf("SAVE TOKENS: %s", saveTokens[3]);

    connectTCP();

    //printf("O SAVE TOKENS E: %s", saveTokens[3]);
    sprintf(file_with_extension, "%s.txt", saveTokens[2]); //adding the .txt to the file name;
    //strcat(saveTokens[2], ".txt"); //adding the .txt to the file name
    printf("THE FILE NAME IS: %s\n", file_with_extension);
    stat(file_with_extension, &qsize);
    var = qsize.st_size; //Size of text doc
    var += 1; //e preciso por causa do \n??
    
    sprintf(message, "QUS %s %s %s %ld ", id_user, local_topic, saveTokens[1], var);

    while(var > 0){
        numBytes = treatBufferDataQUS(file_with_extension, var, indice, message);
        var = var - numBytes;
        indice += numBytes;
        memset(message, 0, 1024);
    }

    strcpy(message, "");
    if (numTokens == 4){
        strcat(message, " 1 ");
        const char ch = '.';
        char* ext = (char*)malloc(sizeof(char)*4);
        ext = strchr(saveTokens[3], ch);  

        char var2[1024];
        int size_image;
        printf("The ext is: %s", ext);
        printf("The message is: %s", message);
        strcat(message, ext); // Extension of image
        strcat(message, " ");
        stat(saveTokens[3], &isize);
        sprintf(var2, "%ld", isize.st_size);
        size_image = atoi(var2);//Size of image
        strcat(message, var2);
        strcat(message, " ");
        indice = 0;
        while(size_image > 0){
            numBytes = treatBufferImageQUS(saveTokens, size_image, indice, message);
            size_image = size_image - numBytes;
            indice += numBytes;
            memset(message, 0, 1024);
        }
    }else {
        strcat(message, " 0");
    }
    strcat(message, "\n\0"); //atencao ao \0
    //send_commandTCP(message);
}

void send_message_as(char* message, int numTokens, char** saveTokens){
    long var;
    char buffer[1024];
    memset(buffer, 0, 1024);
    strcpy(buffer, "");
    //char buffer[DEFAULT_BUFFER_SIZE];
    //int offset = 0;
    int indice = 0;
    int numBytes; 

    connectTCP();

    strcat(saveTokens[2], ".txt"); //adding the .txt to the file name
    stat(saveTokens[2], &qsize);
    var = qsize.st_size; //Size of text doc
    var += 1; //e preciso por causa do \n??
    
    sprintf(message, "ANS %s %s %s ", id_user, local_topic, local_question);
    
    while(var > 0){
        numBytes = treatBufferDataQUS(saveTokens, var, indice, message);
        var = var - numBytes;
        indice += numBytes;
        memset(message, 0, 1024);
    }
    
}

void send_message_err(char* message){
    strcpy(message, "ERR\n");
    send_commandUDP(message);
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
	if(strcmp(id_user,"flag")){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void topics_print(char** saveTokens){
    int i;
    int number = atoi(saveTokens[1]);
    create_directory("TOPICS"); 

    for(i = 1; i <= number; i++){
        //printf("%d - ", i);
        char * token = strtok(saveTokens[i+1], ":");
        //printf("%s ", token);
        char * token2 = strtok(NULL, "");
        //printf("(proposed by %s)\n", token2);
        create_topic_directory(token, token2);
    }
    topicList();
}

void questions_print(char** saveTokens){
    int i;
    int number = atoi(saveTokens[1]); 
    if(number == 0){
        printf("No questions available\n");
    }
    else{
        printf("available questions about %s:\n", local_topic);
    }
    for(i = 1; i <= number; i++){
        printf("%d - ", i);
        char * token = strtok(saveTokens[i+1], ":");
        printf("%s ", token);
        char * token2 = strtok(NULL, ":");
        printf("(proposed by %s)\n", token2);
        //char * token3 = strtok(NULL, ""); //number of answers
        create_question_directory(token, token2);
    }
}

int treatBufferDataQUS(char** file_with_extension, int qsize, int indice, char* message){
    FILE* fd;
    int max = qsize > 1024 - strlen(message) ? 1024 - strlen(message) : qsize;
    char* newMessage = (char*)malloc(sizeof(char)*max);

    fd = fopen(file_with_extension, "r");
    if (fd == NULL){
        fprintf(stderr, "cannot open input file\n");
        return -1;
    }
    fseek(fd, indice, SEEK_SET);
    fread(newMessage, 1, max, fd);
    if(max == qsize){
        //return max; 
        //strcat(newMessage, " 0\n"); //sem imagem
    }
    strcat(message, newMessage);
    writeTCP(message);
    return max;
}

int treatBufferImageQUS(char** saveTokens, int qsize, int indice, char* message){
    FILE* fd;
    int max = qsize > 1024 - strlen(message) ? 1024 - strlen(message) : qsize;
    char* newMessage = (char*)malloc(sizeof(char)*max);
 
    fd = fopen(saveTokens[3], "rb");
    if (fd == NULL){
        fprintf(stderr, "cannot open input file\n");
        return -1;
    }
    fseek(fd, indice, SEEK_SET);
    fread(newMessage, 1, max, fd); 
    if(max == qsize){
        printf("YO\n");
    }
    strcat(message, newMessage);
    writeTCP(message);
    return max;
}