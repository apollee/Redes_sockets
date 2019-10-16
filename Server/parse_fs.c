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
#include "directory_structure_fs.h"

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
    else{
        strcpy(message, "ERR\n");
    }
    char* finalMessage = realloc(message, strlen(message)+1);
    return finalMessage;
}

int isREG(const char* ip){
	//return searchIP(headUser, ip);
    return TRUE;
}

char** parse_commandTCP(char* message){
    int nSpaces = 0;
    int j = 0;
    int k = 0;
    int i;
    char** saveTokens = saveTokensInit(6, 50);
    //First part of parse until data
    for(i = 0; i < 50; i++){
        if(message[i] == ' '|| message[i] == '\n'){
            saveTokens[j][k] = '\0';
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
    if(j == 2){
        j++;
    }
    sprintf(saveTokens[j],"%d", i);
    return saveTokens;
}

int treatBufferData(char** saveTokens, int ind, int num, char* buffer){
    
    int max = num > strlen(buffer) ? strlen(buffer) : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind+1));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    message[k] = '\0';
    writeFileData(saveTokens, message);
    return i;
}

int treatBufferImg(char** saveTokens,int ind, int num, long int n, char* buffer, char* ext){
    int max = num > n ? n : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    //printf("%s\n",message);
    writeFileImg(saveTokens, message, ext, max-ind);
    return i;
}

char** saveTokensInit(int row, int collumn){
    char** saveTokens = (char **) malloc(sizeof (char*) * row);
    for(int i = 0; i < collumn; i++){
        saveTokens[i] = (char *) malloc(sizeof(char)*collumn);
        memset(saveTokens[i], 0, collumn);
    }
    return  saveTokens;
}

void parseQGU(char** saveTokens, int socket, char* buffer){
        int indice = 0;
        int numBytes;
        char * message = (char*)malloc(sizeof(char)*1024);
        memset(message, 0, 1024);
        char* qUser = questionID(saveTokens[1], saveTokens[2]);
        char* qSize = questionTextSize(saveTokens[1],saveTokens[2]);
        //char* qImg = checkQuestionImage(saveTokens[1], saveTokens[2]);
        strcpy(message, "GQR ");
        strcat(message, qUser);
        strcat(message, " ");
        strcat(message, qSize);
        strcat(message, " ");
        while(qSize > 0){
            numBytes = treatBufferDataQGU(saveTokens, atoi(qSize), indice, socket, message);
            qSize -= numBytes;
            indice += numBytes;
        }


}

int treatBufferDataQGU(char** saveTokens, int qSize, int indice, int socket, char* message){
    FILE* file;
    int max = qSize > 1024-strlen(message) ? 1024-strlen(message) : qSize; 
    char* newMessage = (char*)malloc(sizeof(char)*(max));
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.txt",saveTokens[1],saveTokens[2], saveTokens[2]);
    file = fopen(path, "r");
    if(file < 0){
        exit(1);
    }
    fseek(file, indice, SEEK_SET);
    fread(newMessage,1, max, file);
    strcat(message, newMessage);
    write(socket, message, strlen(message));
    return (indice + max);
}

char* parseANS(char**saveTokens, int newfd, ssize_t n, char* buffer, char* message){
    int indice;
    int num = atoi(saveTokens[4]);
    int i = atoi(saveTokens[5]);
    char* path = (char*)malloc(sizeof(char)*50);
    memset(path,0,50);
    strcpy(path, "TOPICS/");
    strcat(path,saveTokens[2]);
    strcat(path, "/");
    strcat(path, saveTokens[3]);
    
    if(atoi(numberOfdirectories(path))==99){
        strcpy(message, "ANR FUL\n");
        return message;
    }
    else{
        char* number = createAnswer(saveTokens);
        while(num > 0){
            indice = treatBufferDataANS(saveTokens, i, num, buffer, number);
            num = num - (indice - i);
            i = 0;
            if(indice == strlen(buffer)){
                memset(buffer, 0, 1024);
                n = read(newfd, buffer, 1024);
            }
        } 
        if(buffer[indice] == ' '){
            indice++;
            if(buffer[indice] == '0'){
                strcpy(message, "ANR OK\n");
            }
            else{
                indice+=2;
                char* ext = (char*)malloc(sizeof(char)*3);
                int j;
                for(j = 0; j<3; j++, indice++){
                    ext[j] = buffer[indice];
                }
                ext[j] = '\0';
                indice ++;
                char* isize = (char*)malloc(sizeof(char)*10);
                memset(isize, 0, 10);
                int indImg = 0;  
                while(buffer[indice] != ' '){
                    isize[indImg] = buffer[indice];
                    indice++;
                    indImg++;
                }
                isize[indImg] = '\0';
                indice++;
                i = indice;
                int numImg = atoi(isize);
                while(numImg > 0){
                    indice = treatBufferImgANS(saveTokens, i, numImg, n, buffer, ext, number);
                    numImg = numImg - (indice - i);
                    i = 0;
                    if(indice == n){
                        memset(buffer, 0, 1024);
                        n = read(newfd, buffer, 1024);
                    }
                }
             strcpy(message, "ANR OK\n");
             printf("DATA SEND\n");
            }
        }
    }
    return message;    
}

int treatBufferDataANS(char** saveTokens, int ind, int num, char* buffer, char* number){
    
    int max = num > strlen(buffer) ? strlen(buffer) : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind+1));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    message[k] = '\0';
    writeFileDataANS(saveTokens, message, buffer, number);
    return i;
}

int treatBufferImgANS(char** saveTokens,int ind, int num, long int n, char* buffer, char* ext, char* number){
    int max = num > n ? n : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    //printf("%s\n",message);
    writeFileImgANS(saveTokens, message, ext, max-ind, number);
    return i;
}