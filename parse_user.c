#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include "user.h"
#include "parse_user.h"
#include "commands_user.h"
#include "directory_structure_user.h"

ssize_t n;

void input_command_user(int argc, char *argv[], char *port, char *ip) {

    if(argc == 1){
        return;
    }
    else if(argc == 3 && !(strcmp(argv[1],"-n"))) {
        strcpy(ip,argv[2]);
    }
    else if(argc == 3 && !(strcmp(argv[1],"-p"))) {
        strcpy(port,argv[2]);
    }
    else if(argc == 5 && !(strcmp(argv[1],"-n")) && !(strcmp(argv[3],"-p"))) {
        strcpy(ip, argv[2]);
        strcpy(port,argv[4]);
    }
    else{
        fprintf(stderr, "Invalid syntax!\n");
        exit(-1);
    }
}

int parse_command() { // command that the user wrote

    int numTokens = 0;
    char *saveTokens[7];
    char input[50];
    int numberChar;
    if(fgets(input, 50, stdin) == NULL){
        return -1;
    }    

    numberChar = strlen(input);
    input[strcspn(input, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(input, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action(numTokens, saveTokens, input, numberChar);
    return 0;
}

//action that the user requested
void input_action(int numTokens, char** saveTokens, char* input, long int numberChar){ 
    char* message = (char*)malloc(sizeof(char)*1024); 
    memset(message, 0, 1024);
    strcpy(message, "");

    //--------------------------------------------------------------------
    // UDP CMDS
    //--------------------------------------------------------------------
    if((!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg"))) {
        if(commandREGOK(numTokens, saveTokens, numberChar))
            send_message_reg(saveTokens[1], message);
        else
            printf("Invalid arguments\n"); //nao devia ser um ERR tambem?
    }

    else if((!strcmp(saveTokens[0], "topic_list") || !strcmp(saveTokens[0], "tl"))) {
        if(commandTLOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
            	printf("You should register first\n");
            else
                send_message_tl(message);
        }
        else
            printf("Invalid arguments\n"); 
    }

    else if((!strcmp(saveTokens[0], "topic_select") || !strcmp(saveTokens[0], "ts"))) {
        if(commandTSOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
            	printf("You should register first\n");
            else
            	printf("selected topic: %s (%s)\n", local_topic, topicID(local_topic));
        }else
            printf("Invalid arguments\n");
    } 

    else if((!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp"))) {
        if(commandTPOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else
                send_message_tp(saveTokens[1], message);
        }else{
            printf("Invalid arguments\n");
        }
    }

    else if((!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql"))){
        if(commandQLOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else if (!strcmp(local_topic, FLAG))
                printf("You have to select a topic first\n");
            else
                send_message_ql(message);
        }else{
            printf("Invalid arguments\n");
        }
    }
    //--------------------------------------------------------------------
    // TCP CMDS
    //--------------------------------------------------------------------
    else if((!strcmp(saveTokens[0], "question_get") || !strcmp(saveTokens[0], "qg"))){
        if(commandQGOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else
                send_message_qg(message);
        }else{
            printf("Invalid arguments\n");
        }
    }
    
    else if((!strcmp(saveTokens[0], "question_submit") || !strcmp(saveTokens[0], "qs"))){
        if(commandQSOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else{
                send_message_qs(message, numTokens, saveTokens);
            }
        }else{
            printf("Invalid arguments\n");
        }
    }

    else if((!strcmp(saveTokens[0], "answer_submit") || !strcmp(saveTokens[0], "as"))){
        if(commandASOK(numTokens, saveTokens, numberChar)){
            if(!isREG(id_user))
                printf("You need to register first\n");
            else
                send_message_as(message, numTokens, saveTokens);
        }else{
            printf("Invalid arguments\n");
        }
    }

    else if(!strcmp(saveTokens[0], "exit"))
        exit(0);

    else
        //send_message_err(message);
       printf("Invalid command\n");
    
    free(message);
}

//comand that the user got from the server
void parse_command_received(char* buffer){ 
    int numTokens = 0;
    int i;
    //char *saveTokens[120];
    int numberChar;

    char** saveTokens = (char **) malloc(sizeof (char*) * 120);
    
    for(i = 0; i < 120; i++){
        saveTokens[i] = (char *) malloc(sizeof(char)*100);
        memset(saveTokens[i], 0, 100);
    }
    
    numberChar = strlen(buffer);
    buffer[strcspn(buffer, "\n")] = 0; 
    char *token = strtok(buffer, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    //free(buffer);
    input_action_received(numTokens, saveTokens, numberChar);
    /*for(i = 0; i < 120; i++){
        free(saveTokens[i]);
    }*/
    free(saveTokens);
    free(token);
}

//UDP message from the server
void input_action_received(int numTokens, char** saveTokens, long int numberChar){ 
    char command[5];
    strcpy(command, saveTokens[0]);

    if((!strcmp(command,"RGR")) && numTokens == 2) {
        if(!strcmp(saveTokens[1], "OK")){
            printf("User \"%s\" registered\n", id_user);
        }
        else if(!strcmp(saveTokens[1], "NOK")){
            printf("User %s not registered\n", id_user); //e suposto?
        }
    }
    else if(!strcmp(command, "LTR")){ 
        printf("available topics:\n");
        topics_print(saveTokens);
    }
    else if(!strcmp(command, "PTR")){
       printf("%s %s\n", saveTokens[0], saveTokens[1]);
       
    }
    else if(!strcmp(command, "LQR")){
        questions_print(saveTokens);
    }
    else{
        printf("Unexpected server response\n");
    }
}

char** parse_command_received_TCP(char* message){
    int nSpaces = 0;
    int j = 0;
    int k = 0;
    int i;
    char** saveTokens = saveTokensInit(4, 50);
    //First part of parse until data
    for(i = 0; i < 50; i++){
        if(message[i] == ' '){
            saveTokens[j][k] = '\0';
            nSpaces++;
            j++;
            k = 0;
        }
        else if(nSpaces == 3){
            break;
        }
        else{
            saveTokens[j][k] = message[i];
            k++;
        }
    }
    sprintf(saveTokens[j],"%d", i);
    input_action_received_TCP(saveTokens);
    return saveTokens;
}

//TCP message from the server
void input_action_received_TCP(char** saveTokens){ 
    char command[5];
    char* buffer = (char*)malloc(sizeof(char)* 1024);
    memset(buffer, 0, 1024);
    strcpy(command, saveTokens[0]);

    if(!strcmp(command, "QGR")){
        int qUserID = atoi(saveTokens[1]);
        int qSize = atoi(saveTokens[2]);
        int firstOffset = atoi(saveTokens[5]);
        int indice;
        char* path = (char*)malloc(sizeof(char)* 1024);
        sprintf(path, "TOPICS/%s/%s", local_topic, local_question);
        
        

        while(qSize > 0){
            indice = treatBufferData(saveTokens, firstOffset, qSize, buffer);
            qSize = qSize- (indice - firstOffset);
            firstOffset = 0;

            if(indice == strlen(buffer)){
                memset(buffer, 0, 1024);
                //n = read(newfd, buffer, 1024);
                buffer = readTCP();
                indice = 0;
            }
        }











        
        printf("localTopic: %s\n", local_topic);
        printf("localQuestion: %s\n", local_question);




        


        // //calculating qdata
        // int offset = strlen(saveTokens[0]) + strlen(saveTokens[1]) + strlen(saveTokens[2]) + 3 ;
        





        // while(qsize > 0){
        //     indice = treatBufferData(saveTokens, offset, qsize, buffer); //qdata
        //     qsize = qsize - (indice - qdata);    
        //     qdata = 0;

        //     if(indice == strlen(buffer)){
        //         memset(buffer, 0, 1024);
        //         n = read(fdTCP, buffer, 1024);
        //     }
        // }

        // if(buffer[indice] == ' '){
        //     indice++;
        //     if(buffer[indice] == '0'){
        //         indice++;
        //         //printf("Got the file of the question with success\n");
        //     }else{
        //         indice = parse_image_qg(indice, buffer); //qiext qisize qidata
        //     }
        // }

        // if(buffer[indice] == ' '){
        //     indice++;
            
        //     // char* number; 
        //     // while(buffer[indice] != ' '){
        //     //     strcat(number, buffer+indice);
        //     //     indice++;
        //     // }   

        //     int number_answers = atoi(number); // N
        //     indice += 2;
        //     while(number_answers > 0){
        //         indice = parse_answers_qg(indice, buffer); //aUserID asize adata
        //         if(buffer[indice] == ' '){
        //             indice++;
        //             if(buffer[indice] == '0'){
        //                 indice++;
        //             }else{
        //                 indice = parse_answers_image_qg(indice, buffer); //aiext AN aisize aidata
        //             }
        //         }
        //     }
        // }
        // /*funcao missing*/        
    }

    else if(!strcmp(command, "QUR")){
        if(!strcmp(saveTokens[1], "OK")){
            printf("Question submitted with success\n");
        }else if(!strcmp(saveTokens[1], "NOK")){
            printf("Failed to submit question\n");
        }else if(!strcmp(saveTokens[1], "FUL")){
            printf("Failed to submit question - Maximum limit of questions reached\n");
        }else if(!strcmp(saveTokens[1], "DUP")){ 
            printf("Duplicated question, failed to submit\n");
        }
        //printf("%s %s\n", command, saveTokens[1]);
    }
    else if(!strcmp(command, "ANR")){
        if(!strcmp(saveTokens[1], "OK")){
            printf("Answer submitted with success\n");
        }else if(!strcmp(saveTokens[1], "NOK")){
            printf("Failed to submit answer\n");
        }else if(!strcmp(command, "FUL")){
            printf("Failed to submit answer - Maximum limit of answers reached\n");
        }
        printf("%s %s\n", command, saveTokens[1]);
    }
    else{
        printf("Unexpected server response\n");
    }
    free(buffer);
}

int parse_image_qg(int indice, char* buffer){
    indice += 2;
    char* ext = (char*)malloc(sizeof(char)*3);
    int j;
    for(j = 0; j < 3; j++, indice++){ //checkar a extensao
        ext[j] = buffer[indice];
    }
    ext[j] = '\0';
    indice++;
    char* qisize = (char*)malloc(sizeof(char)*10);
    memset(qisize, 0, 10);
    int indImg = 0;
    while(buffer[indice] != ' '){ //guardar o isize
        qisize[indImg] = buffer[indice];
        indice++;
        indImg++;
    }
    qisize[indImg] = '\0';
    indice++;
    int i = indice;
    int numImg = atoi(qisize); //image size
    while(numImg > 0){
        indice = treatBufferImg(i, numImg, n, buffer, ext);
        numImg = numImg - (indice - i);
        i = 0;
        if(indice == n){
            memset(buffer, 0, 1024);
            n = read(fdTCP, buffer, 1024);
        }
    }
    return indice;
}

int parse_answers_image_qg(int indice, char* buffer){
    indice += 2;
    char* ext = (char*)malloc(sizeof(char)*3);
    int j;
    for(j = 0; j < 3; j++, indice++){ //checkar a extensao
        ext[j] = buffer[indice];
    }
    ext[j] = '\0';
    indice++;

    char* an = (char*)malloc(sizeof(char)*2);
    memset(an, 0, 2);
    //int indAn = 0;
    for(j = 0; j < 2; j++, indice++){ //guardar o AN
        an[j] = buffer[indice];

    }
    an[j] = '\0';
    indice++;

    char* qisize = (char*)malloc(sizeof(char)*10);
    memset(qisize, 0, 10);
    int indImg = 0;
    while(buffer[indice] != ' '){ //guardar o qsize
        qisize[indImg] = buffer[indice];
        indice++;
        indImg++;
    }
    qisize[indImg] = '\0';
    indice++;
    int i = indice;
    int numImg = atoi(qisize); //image size
    while(numImg > 0){
        indice = treatBufferImg(i, numImg, n, buffer, ext); //vai ter que ser um treatbufferimg2
        numImg = numImg - (indice - i);
        i = 0;
        if(indice == n){
            memset(buffer, 0, 1024);
            n = read(fdTCP, buffer, 1024);
        }
    }
    return indice;    
}


int parse_answers_qg(int indice, char* buffer){
    char* id_answer = (char*)malloc(sizeof(char)*5);
    int j;
    for(j = 0; j < 5; j++, indice++){
        id_answer[j] = buffer[indice];
    }
    id_answer[j] = '\0';
    indice++;
    char* asize = (char*)malloc(sizeof(char)*10);
    memset(asize, 0, 10);
    int indData = 0;
    while(buffer[indice] != ' '){
        asize[indData] = buffer[indice];
        indice++;
        indData++;
    }
    asize[indData] = '\0';
    indice++;
    int i = indice;
    int numData = atoi(asize);
    while(numData > 0){
        indice = treatBufferData2(i, numData, buffer); //esta funcao nao da como esta, nova ou mudar esta?
        numData = numData - (indice - i);
        i = 0;
        if(indice == strlen(buffer)){
            memset(buffer, 0, 1024);
            n = read(fdTCP, buffer, 1024);
        }
    }
    return indice;

}

int treatBufferImg(int ind, int num, long int n, char* buffer, char* ext){
    int max = num > n ? n : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    //printf("%s\n",message);
    writeFileImg(message, ext, max-ind);
    return i;
}

int treatBufferData(char** saveTokens, int ind, int qsize, char* buffer){
    
    int max = qsize > strlen(buffer) ? strlen(buffer) : qsize;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind+1));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    message[k] = '\0';
    writeFileData(message);
    return i;
}

int treatBufferData2(int ind, int num, char* buffer){
    int max = num > strlen(buffer) ? strlen(buffer) : num;
    int i, k = 0;
    char* message = (char*)malloc(sizeof(char)*(max-ind+1));
    memset(message, 0, max-ind);
    for(i = ind; i < max; i++, k++){
        message[k] = buffer[i];
    }
    message[k] = '\0';
    writeFileData2(message);
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