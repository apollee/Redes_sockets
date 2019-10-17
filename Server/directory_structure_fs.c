#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "directory_structure_fs.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

//MAIN FS---------------------------------------------------------
int check_directory_existence(char *dirname){
    DIR *d;
    d = opendir(dirname);

    if(d){
        closedir(d);
        return TRUE;
    }else{
        return FALSE;
    }
}

void create_directory(char* dirname){
    mkdir(dirname, 0700);
}

//TOPIC LIST------------------------------------------------------
char* number_of_topics(){ //get the number of total topics
    char* value = (char*)malloc(sizeof (char)* 1024);
    char* finalValue;
    int number = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir("TOPICS");

    if (d){
        while((dir=readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                number++;
            }
        }
        closedir(d);
    }
    else 
        return NULL;  
    
    sprintf(value, "%d", number);
    finalValue = realloc(value, strlen(value)+1);
    return finalValue;
}

char* topicList(char* numTopics){ //get the list of topics
    DIR *d;
    struct dirent *dir;
    d = opendir("TOPICS");
    char* message = (char*)malloc (sizeof (char)* 1024);
    memset(message, 0, 1024);
    char* finalMessage;
    strcpy(message, "");
    int num = atoi(numTopics);
    int count = 0;
    //printf("message antes do topicList: !%s!\n", message);
    if (d){
        dir = readdir(d);
        while(dir != NULL){
            //printf("dirName: %s\n", dir->d_name);
           if ((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                //printf("currMessage: !%s!\n", message);
                strcat(message, dir->d_name);
                strcat(message, ":");
                strcat(message, topicID(dir->d_name));
                count++;
                if(num != count){
                    strcat(message, " ");
                }
            }
            dir=readdir(d);
        }
        closedir(d);
        finalMessage = realloc(message, strlen(message)+1);
        return finalMessage;
    }
    else 
        return NULL;
}

char* topicID(char* dirname){ //get the id of the person that created a topic
    char* message = (char*)malloc (sizeof (char)* 1024); 
    sprintf(message, "TOPICS/%s/%s_UID.txt", dirname, dirname);
    FILE* file;
    char* id = (char*) malloc(sizeof(char)*6);
    file = fopen(message, "r");
    if(file == NULL){
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%5s", id);
    
    free(message);
    return id;
}
//TOPIC PROPOSE---------------------------------------------------

int check_max_directory_size(char* path){
    if (atoi(numberOfdirectories(path)) < 99)
        return TRUE;
    else
        return FALSE;
}

char* numberOfdirectories(char* path){
    int number = 0;
    DIR *d;
    struct dirent *dir;
    char* value = (char*)malloc(sizeof (char)* 1024);
    memset(value, 0, 1024);
    char* finalValue;
    d = opendir(path);
    //free(path);
    if (d){
        while((dir=readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, ".")) && dir->d_type == DT_DIR){             
                number++;
            }
        }
        closedir(d);
    }
    else{
        sprintf(value, "%d", -1);
        return value;
    }
    sprintf(value, "%d", number);
    finalValue = (char*)realloc(value, strlen(value)+1);
    return finalValue;
}

void create_topic_directory(char *dirname, char* userID){
    DIR *d;
    //Open topic folder
    d = opendir("TOPICS");
    int fd = dirfd(d);
    mkdirat(fd, dirname, 0700);
    
    //Criar ficheiro
    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s/%s_UID.txt", dirname, dirname);
    file = fopen(path, "w");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    strcat(userID, "\0");
    fprintf(file,"%s", userID);
    fclose(file);
    free(path);
    closedir(d);
}

//QUESTION LIST---------------------------------------------------

char* questionList(char* currTopic, char* numDirec){
    char* path = (char*)malloc(sizeof (char)* 1024);
    DIR *d;
    struct dirent *dir;
    sprintf(path, "TOPICS/%s/", currTopic);
    d = opendir(path);
    int num = atoi(numDirec);
    int count = 0;
    char* message = (char*)malloc (sizeof (char) * 1024);
    memset(message, 0, 1024);
    char* finalMessage;
    strcpy(message, " ");

    char* userID = (char*)malloc(sizeof (char)* 1024);
    sprintf(userID, "%s_UID.txt", currTopic);
    if (d){
        dir=readdir(d);
        while(dir != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, ".")) && (strcmp(dir->d_name, userID))){
                strcat(message, dir->d_name);
                strcat(message, ":");
                strcat(message, questionID(currTopic, dir->d_name));
                strcat(message, ":");
                sprintf(path, "TOPICS/%s/%s/", currTopic, dir->d_name);
                strcat(message, numberOfdirectories(path));
                count++;
                if (num != count){
                    strcat(message, " ");
                } 
            }
            dir=readdir(d);
        }

        free(userID);
        closedir(d);
        finalMessage = (char*)realloc(message, strlen(message)+1);
        free(path);
        return finalMessage;
    }
    else{
        free(userID);
        return NULL;
    }
}


char* questionID(char* currTopic, char* dirname){
    char* path = (char*)malloc (sizeof (char) * 1024); 
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s_UID.txt", currTopic, dirname, dirname);
    FILE* file;
    char* id = (char*)malloc(sizeof(char)*6);
    
    file = fopen(path, "r");
    free(path);
    if(file == NULL){
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%s", id);
    strcat(id, "\0");

    return id;
}

char* questionTextSize(char* currTopic, char* dirname){
    char* path = (char*)malloc (sizeof (char) * 1024); 
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.txt", currTopic, dirname, dirname);
    FILE* file;
    char* qsize = (char*)malloc(sizeof(char)*11);
    memset(qsize, 0, 11);
    
    file = fopen(path, "r");
    free(path);
    if(file == NULL){
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    sprintf(qsize, "%ld", size);

    return qsize;
}

char* checkQuestionImage(char *currTopic, char* dirname){
    char* path = (char*)malloc (sizeof (char) * 1024); 
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s", currTopic, dirname);
    
    FILE* file;
    char* qsize = (char*)malloc(sizeof(char)*2);
    memset(qsize, 0, 2);
    file = fopen(path, "r");
    free(path);
    if(file == NULL){
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    sprintf(qsize, "%ld", size);

    return qsize;
}

int checkExistenceofTopic(char* dirname){ //check if a topic exists
    DIR *d;
    struct dirent *dir;
    d = opendir("TOPICS");

    if (d){
        while((dir=readdir(d)) != NULL){
            if(!(strcmp(dir->d_name, dirname))){
                return 1;
            }
        }
        closedir(d);
    }
    return 0;
}

//QUESTION SUBMIT-------------------------------------------------
void createQuestion(char* pathTopic, char** saveTokens){
    DIR *d;
    //Open topic folder
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    strcpy(path, "TOPICS/");
    strcat(path, saveTokens[2]);
    char* newPath = realloc(path, strlen(path) + 1); 
    d = opendir(newPath);
    int fd = dirfd(d);
    mkdirat(fd, saveTokens[3], 0700);
    
    //Criar ficheiro
    FILE* file;
    char* pathDir = (char*)malloc (sizeof (char) * 1024);
    memset(pathDir, 0, 1024);
    sprintf(pathDir, "%s/%s/%s_UID.txt", newPath,saveTokens[3], saveTokens[3]);
    file = fopen(pathDir, "w");
    free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fprintf(file,"%s", saveTokens[1]);
    fclose(file);
    free(pathDir);
    
}
void writeFileData(char ** saveTokens, char* message){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.txt", saveTokens[2], saveTokens[3], saveTokens[3]);
    file = fopen(path, "a");
    free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fprintf(file,"%s", message);
    fclose(file);
    //free(path);
}

void writeFileImg(char ** saveTokens, char* message, char* ext, long int n){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.%s", saveTokens[2], saveTokens[3], saveTokens[3], ext);
    file = fopen(path, "ab");
    free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fwrite(message, n, sizeof(char), file);
    fclose(file);
    //free(path);
}
/*=====================ANSWER SUBMIT=============*/

char* createAnswer(char** saveTokens){
    DIR *d;
    char* number = (char*)malloc(sizeof(char)*3);
    //Open topic folder
    char* path = (char*)malloc(sizeof(char)*1024);
    memset(path, 0, 1024);
    strcpy(path, "TOPICS/");
    strcat(path, saveTokens[2]);
    strcat(path, "/");
    strcat(path,saveTokens[3]);
    char* newPath = realloc(path, strlen(path) + 1); 
    number = numberOfdirectories(path);
    d = opendir(newPath);
    int fd = dirfd(d);
    if(strlen(number)!=2){
        number[1] = number[0];
        number[0] = '0';
    }
    char * dirName = (char*)malloc(sizeof(char)*20);
    memset(dirName, 0, 20);
    sprintf(dirName,"%s_%s", saveTokens[3], number);
    mkdirat(fd, dirName, 0700);
    
    //Criar ficheiro
    FILE* file;
    char* pathDir = (char*)malloc (sizeof (char) * 1024);
    memset(pathDir, 0, 1024);
    sprintf(pathDir, "TOPICS/%s/%s/%s_%s/%s_%s_UID.txt", saveTokens[2],saveTokens[3], saveTokens[3], number, saveTokens[3], number);
    file = fopen(pathDir, "w");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fprintf(file,"%s", saveTokens[1]);
    fclose(file);
    free(path);
    return number;
    
}

void writeFileDataANS(char ** saveTokens, char* message, char* buffer, char* number){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s_%s/%s_%s.txt", saveTokens[2], saveTokens[3], saveTokens[3], number, saveTokens[3], number);
    file = fopen(path, "a");
    free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fprintf(file,"%s", message);
    fclose(file);
    //free(path);
}
void writeFileImgANS(char ** saveTokens, char* message, char* ext, long int n, char* number){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s_%s/%s_%s.%s", saveTokens[2], saveTokens[3], saveTokens[3], number,saveTokens[3], number, ext);
    file = fopen(path, "ab");
    free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fwrite(message, n, sizeof(char), file);
    fclose(file);
    //free(path);
}