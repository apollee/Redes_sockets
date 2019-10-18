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
    
    int max = num > strlen(buffer)-ind ? strlen(buffer)-ind : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max+1));
    memset(message, 0, max+1);
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
        char* qSize = questionTextSize(saveTokens[1],saveTokens[2], "txt");
        char** Img = checkQuestionImage(saveTokens[1], saveTokens[2]);
        strcpy(message, "QGR ");
        strcat(message, qUser);
        strcat(message, " ");
        strcat(message, qSize);
        strcat(message, " ");
        int qSizeInt = atoi(qSize);
        while(qSizeInt > 0){
            numBytes = treatBufferDataQGU(saveTokens, qSizeInt, indice, socket, message, "txt", "r");
            qSizeInt = qSizeInt - numBytes;
            indice += numBytes;
            memset(message, 0, 1024);
        }
       if(strcmp(Img[0], "0")){
            char* iSize = questionTextSize(saveTokens[1], saveTokens[2], Img[1]);
            memset(message, 0, 1024);
            sprintf(message," %s %s %s ",Img[0], Img[1], iSize);
            int iSizeInt = atoi(iSize);
            write(socket, message, strlen(message));
            memset(message, 0, 1024);
            indice = 0;
            while(iSizeInt > 0){
                numBytes = treatBufferDataImgQGU(saveTokens, iSizeInt, indice, socket, message, Img[1], "rb");
                iSizeInt = iSizeInt - numBytes;
                indice += numBytes;
               
            }
        }else{
            memset(message,0,1024);
            sprintf(message," %s",Img[0]);
            write(socket, message, strlen(message));
            memset(message, 0, 1024);
        }
        char* numbAns = number_of_answers(saveTokens[1], saveTokens[2]);
        if(strcmp(numbAns,"0")){
            sprintf(message, " %s", numbAns);
            write(socket, message, strlen(message));
            memset(message, 0, 1024);
            int numAnsInt = atoi(numbAns);
            int N = 10 > numAnsInt ? numAnsInt : 10;
            for(int i = 0; i < N; i++){
                if(N-i < 10){
                    sprintf(numbAns,"0%d", N-i);
                }
                else{
                    sprintf(numbAns,"%d", N-i);
                }
                char* aUserID = answerID(saveTokens[1], saveTokens[2], numbAns);
                char* asize = questionAnswerSize(saveTokens[1], saveTokens[2], numbAns, "txt");
                char** aImg = checkAnswerImage(saveTokens[1], saveTokens[2], numbAns);
                
                sprintf(message, " %s %s %s ",numbAns, aUserID, asize);
                write(socket, message, strlen(message));
                memset(message, 0, 1024);
                int asizeInt = atoi(asize);
                int indiceAns = 0;
                while(asizeInt > 0){
                    numBytes = treatBufferDataQGUANS(saveTokens, asizeInt, indiceAns, socket, message, "txt", "r",numbAns);
                    asizeInt = asizeInt - numBytes;
                    indiceAns += numBytes;
                    memset(message, 0, 1024);
                }
                if(strcmp(aImg[0], "0")){
                    char* aiSize = questionAnswerSize(saveTokens[1], saveTokens[2],  numbAns, aImg[1]);
                    sprintf(message," %s %s %s ",aImg[0], aImg[1], aiSize);
                    int aiSizeInt = atoi(aiSize);
                    write(socket, message, strlen(message));
                    memset(message, 0, 1024);
                    int indiceIAns = 0;
                    while(aiSizeInt > 0){ 
                        numBytes = treatBufferDataImgQGUANS(saveTokens, aiSizeInt, indiceIAns, socket, message, aImg[1], "rb", numbAns);
                        aiSizeInt = aiSizeInt - numBytes;
                        indiceIAns += numBytes;

                    }
                }
                else{
                    //sprintf(message," %s",aImg[0]);
                    strcpy(message," 0");
                    write(socket, message, strlen(message));
                    memset(message, 0, 1024);
                }
            }
            strcpy(message,"\n");
            write(socket, message,strlen(message));

        }
       else{
            strcpy(message, " 0\n");
            write(socket, message, strlen(message));
            memset(message, 0, 1024);
       }

}


int treatBufferDataQGUANS(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char* perm, char* numbAns){
    FILE* file;
    int max = qSize > 1024 ? 1024 : qSize; 
    char* newMessage = (char*)malloc(sizeof(char)*(max));
    memset(newMessage,0,max);
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s_%s/%s_%s.%s",saveTokens[1],saveTokens[2], saveTokens[2],numbAns,saveTokens[2], numbAns, ext);
    file = fopen(path, perm);
    if(file < 0){
        exit(1);
    }

    fseek(file, indice, SEEK_SET);
    int n = fread(newMessage, 1, max, file);
    ssize_t N = write(socket, newMessage, n);
    fclose(file);
    free(path);
    return N;
}

int treatBufferDataImgQGUANS(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char* perm, char* numbAns){
    FILE* file;
    ssize_t n;
    int max = qSize > 1024 ? 1024 : qSize; 
    char* newMessage = (char*)malloc(sizeof(char)*(max));
    memset(newMessage, 0, max);
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s_%s/%s_%s.%s",saveTokens[1],saveTokens[2], saveTokens[2],numbAns,saveTokens[2], numbAns, ext);
    file = fopen(path, perm);
    if(file < 0){
        exit(1);
    }
    fseek(file, indice, SEEK_SET);
    int nread = fread(newMessage, 1, max, file);
    n = write(socket, newMessage, nread);
    fclose(file);
    free(path);
    return n;
}
int treatBufferDataQGU(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char* perm){
    FILE* file;
    int mensSize = strlen(message);
    int max = qSize > 1024 ? 1024-strlen(message) : qSize; 
    char* newMessage = (char*)malloc(sizeof(char)*(max));
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.%s",saveTokens[1],saveTokens[2], saveTokens[2], ext);
    file = fopen(path, perm);
    if(file < 0){
        exit(1);
    }
    fseek(file, indice, SEEK_SET);
    int n = fread(newMessage,1, max, file);
    strcat(message, newMessage);
    int N = write(socket, message, n);
    memset(message, 0, 1024);    
    fclose(file);
    free(path);
    return N-mensSize;
}

int treatBufferDataImgQGU(char** saveTokens, int qSize, int indice, int socket, char* message, char* ext, char* perm){
    FILE* file;
    ssize_t n;
    int max = qSize > 1024 ? 1024 : qSize; 
    char* newMessage = (char*)malloc(sizeof(char)*(max));
    memset(newMessage, 0, max);
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.%s",saveTokens[1],saveTokens[2], saveTokens[2], ext);
    file = fopen(path, perm);
    if(file < 0){
        exit(1);
    }
    fseek(file, indice, SEEK_SET);
    int nread = fread(newMessage, 1, max, file);
    n = write(socket, newMessage, nread);
    memset(message, 0, 1024);
    fclose(file);
    free(path);
    return n;
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
    
    int max = num > strlen(buffer)-ind ? strlen(buffer)-ind : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max+1));
    memset(message, 0, max);
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