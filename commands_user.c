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
#include "parse_user.h"

struct stat qsize, isize;
FILE* fd_bufferData;
FILE* fd_bufferImg;

int commandREGOK(int numTokens, char** saveTokens, long int numberChar){
    if(numTokens != 2)
        return FALSE;
    if (strlen(saveTokens[1]) != 5)        
        return FALSE;
    else if(!onlyNumbers(saveTokens[1]))        
        return FALSE;
    else if(numberChar - 2 != strlen(saveTokens[0])+strlen(saveTokens[1])) //check spaces     
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
    connectTCP();
    writeTCP(message, 0);
    
    char* messageReceived = readTCP();
    
    char** saveTokens = (char **) malloc(sizeof (char*) * 3);

    for(int i = 0; i < 50; i++){
        saveTokens[i] = (char *) malloc(sizeof(char)*50);
        memset(saveTokens[i], 0, 50);
    }

    int nSpaces = 0;
    int j = 0;
    int k = 0;
    int i;

    for(i = 0; i < 50; i++){
        if(messageReceived[i] == ' '|| messageReceived[i] == '\n'){
            saveTokens[j][k] = '\0';
            nSpaces++; 
            j++;
            k = 0;
        }
        else if(nSpaces == 3){
            break;
        }
        else{
            saveTokens[j][k] = messageReceived[i];
            k++;
        }
    }
    sprintf(saveTokens[j],"%d", i);
    input_action_received_TCP(saveTokens);

}

void send_message_qs(char* message, int numTokens, char** saveTokens){
    long var;
    char* file_with_extension = (char*)malloc(sizeof(char)*100);
    memset(file_with_extension, 0, 100);
    int indice = 0;
    int numBytes; 

    connectTCP();
    sprintf(file_with_extension, "%s.txt", saveTokens[2]); //adding the .txt to the file name;
    fd_bufferData = fopen(file_with_extension, "r");
    if (fd_bufferData == NULL){
        printf("Error: no text file found.\n");
        exit(-1);
    }

    stat(file_with_extension, &qsize);
    var = qsize.st_size; //Size of text doc
    
    sprintf(message, "QUS %s %s %s %ld ", id_user, local_topic, saveTokens[1], var);

    while(var > 0){
        numBytes = treatBufferDataQUS(file_with_extension, var, indice, message);
        var = var - numBytes;
        indice += numBytes;
        memset(message, 0, 1024);
    }

    if(fclose(fd_bufferData) != 0){
        printf("Error: in communication.\n");
        exit(-1);
    }
    
    strcpy(message, "");
    if (numTokens == 4){
        strcat(message, " 1 ");
        const char ch = '.';
        char* ext = (char*)malloc(sizeof(char)*4);
        ext = strchr(saveTokens[3], ch);  
        ext = ext + 1;

        char var2[1024];
        int size_image; 
        strcat(message, ext); // Extension of image
        strcat(message, " ");
        stat(saveTokens[3], &isize);
        sprintf(var2, "%ld", isize.st_size);
        size_image = atoi(var2);//Size of image
        strcat(message, var2);
        strcat(message, " ");

        write(fdTCP, message, strlen(message));
        memset(message, 0, 1024);
        indice = 0;
        fd_bufferImg = fopen(saveTokens[3], "rb");
        if (fd_bufferImg == NULL){
            printf("Error: no image found.\n");
            exit(-1);
        }
        while(size_image > 0){
            numBytes = treatBufferImageQUS(saveTokens, size_image, indice);
            size_image = size_image - numBytes;
            indice += numBytes;
        }
        fclose(fd_bufferImg);
    }else {
        strcat(message, " 0");
    }
    strcat(message, "\n"); //atencao ao \0
    writeTCP(message, 0);
    free(file_with_extension);
    printf("%s", message);
    parse_command_received_TCP(readTCP());
}

void send_message_as(char* message, int numTokens, char** saveTokens){
    long var;
    char* file_with_extension = (char*)malloc(sizeof(char)*1024);
    memset(file_with_extension, 0, 1024);
    int indice = 0;
    int numBytes; 

    connectTCP();
    sprintf(file_with_extension, "%s.txt", saveTokens[1]); //adding the .txt to the file name;
    stat(file_with_extension, &qsize);
    var = qsize.st_size; 
    //var += 1; \n?? !!!!
    
    sprintf(message, "ANS %s %s %s %ld ", id_user, local_topic, local_question, var);
    fd_bufferData = fopen(file_with_extension, "r");
    if(fd_bufferData == NULL){
        printf("FILE NAME: %s", file_with_extension);
        fprintf(stderr, "cannot open input file\n");
        exit(-1);
    }

    while(var > 0){
        numBytes = treatBufferDataQUS(file_with_extension, var, indice, message);
        var = var - numBytes;
        indice += numBytes;
        memset(message, 0, 1024);
    }

    fclose(fd_bufferData);
    strcpy(message, "");
    if(numTokens == 3){
        strcat(message, " 1 ");
        const char ch = '.';
        char* ext = (char*)malloc(sizeof(char)*4);
        ext = strchr(saveTokens[2], ch);
        ext = ext + 1;

        char var2[1024];
        int size_image;
        strcat(message, ext);
        strcat(message, " ");
        stat(saveTokens[2], &isize);
        sprintf(var2, "%ld", isize.st_size);
        size_image = atoi(var2);
        strcat(message, var2);
        strcat(message, " ");
        write(fdTCP, message, strlen(message));
        memset(message, 0, 1024);
        indice = 0;
        fd_bufferImg = fopen(saveTokens[2], "rb");
        if(fd_bufferImg == NULL){
            fprintf(stderr, "error: %s\n", strerror(errno));
            exit(-1);
        }
        while(size_image > 0){
            numBytes = treatBufferImageQUS(saveTokens, size_image, indice);
            size_image = size_image - numBytes;
            indice += numBytes;
        }
        fclose(fd_bufferImg);
    }else{
        strcat(message, " 0");
    }
    strcat(message, "\n");
    writeTCP(message, 0);
    free(file_with_extension);
    parse_command_received_TCP(readTCP());
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
        //printf("%d - ", i);
        char * token = strtok(saveTokens[i+1], ":");
        //printf("%s ", token);
        char * token2 = strtok(NULL, ":");
        //printf("(proposed by %s)\n", token2);
        //char * token3 = strtok(NULL, ""); //number of answers
        create_question_directory(token, token2);
    }
    questionList();
}

int treatBufferDataQUS(char* file_with_extension, int qsize, int indice, char* message){
    int max = qsize > 1024 ? 1024 - strlen(message) : qsize;
    char* newMessage = (char*)malloc(sizeof(char)*max);

    fseek(fd_bufferData, indice, SEEK_SET);
    fread(newMessage, 1, max, fd_bufferData);
    strcat(message, newMessage);
    write(fdTCP, message, strlen(message));
    //printf("%s", message);
    free(newMessage);
    return max;
}

int treatBufferImageQUS(char** saveTokens, int qsize, int indice){
    FILE* file;
    ssize_t n;
    int max = qsize > 1024 ? 1024 : qsize;
    char* newMessage = (char*)malloc(sizeof(char)*max);
    memset(newMessage, 0, max);

    fseek(fd_bufferImg, indice, SEEK_SET);
    int nread = fread(newMessage, 1, max, fd_bufferImg); 

    file = fopen("teste.jpg", "ab");
    if(file < 0){
        exit(1);
    }
    fwrite(newMessage, 1,nread, file);
    fclose(file);
    n = write(fdTCP, newMessage, nread);
    free(newMessage);
    return n;
}