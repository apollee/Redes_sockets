#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "directory_structure_user.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "commands_user.h"


void create_directory(char* dirname){
    mkdir(dirname, 0700);
}

void create_topic_directory(char *dirname, char* userID){
    DIR *d;
    //Open topic folder 
    d = opendir("TOPICS");
    if(d == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    int fd = dirfd(d);
    if(fd == -1){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    mkdirat(fd, dirname, 0700);
    
    //Criar ficheiro topico_UID
    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s/%s_UID.txt", dirname, dirname);
    file = fopen(path, "w");
    if (file < 0) {
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    strcat(userID, "\0");
    fprintf(file,"%s", userID);
    fclose(file);
    if(closedir(d) == -1){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    free(path);
}

void create_question_directory(char *dirname, char* userID, char* numberOfQuestions){
    DIR *d;
    //Open folder of the current topic
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "TOPICS/%s", local_topic);
    d = opendir(message);
    if(d == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    int fd = dirfd(d);
    if(fd == -1){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    mkdirat(fd, dirname, 0700);
    
    //Criar ficheiro
    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s/%s/%s_UID.txt", local_topic, dirname, dirname);
    file = fopen(path, "w");
    if(file == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }

    strcat(userID, "\0");
    fprintf(file,"%s", userID);
    fclose(file);
    if(closedir(d) == -1){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }   

    //Criar ficheiro de numero de respostas
    path = (char*)malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s/%s/%s_AN.txt", local_topic, dirname, dirname);
    file = fopen(path, "w");
    if(file == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }

    strcat(numberOfQuestions, "\0");
    fprintf(file,"%s", numberOfQuestions);
    fclose(file);
 

}

int getTopic_by_number(int number){ //get the topic by the number
    DIR *d;
    struct dirent *dir;
    int flag = 0;

    if(number > 99 && number == 0){
        return flag;
    } 

    d = opendir("TOPICS");
    if(d == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    int current_topic_number = 1;
    if(d){
        while((dir = readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                if(current_topic_number == number){
                    strcpy(local_topic, dir->d_name); 
                    return 1;
                }else
                    current_topic_number++;            
            }   
        }
        if(closedir(d) == -1){
            printf("Error in receiving information from the server.\n");
            exit(-1);
        } 
    }
    return flag;
}

int getQuestion_by_number(int number){ //get the topic by the number
    DIR *d;
    struct dirent *dir;
    int flag = 0;
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "TOPICS/%s", local_topic);

    if(number > 99 && number == 0){
        return flag;
    } 

    d = opendir(message);
    if(d == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    int current_question_number = 1;
    if(d){
        while((dir = readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, ".")) && (dir->d_type == DT_DIR)){
                if(current_question_number == number){
                    strcpy(local_question, dir->d_name); 
                    return 1;
                }else
                    current_question_number++;            
            }   
        }
        if(closedir(d) == -1){
            printf("Error in receiving information from the server.\n");
            exit(-1);
        }
    }
    free(message);
    return flag;
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

int checkExistenceofQuestion(char* dirname){ //check if a question exists
    DIR *d;
    struct dirent *dir;
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "TOPICS/%s", local_topic);
    d = opendir(message);

    if (d){
        while((dir=readdir(d)) != NULL){
            if(!(strcmp(dir->d_name, dirname))){
                return 1;
            }
        }
        if(closedir(d) == -1){
            printf("Error in receiving information from the server.\n");
            exit(-1);
        }
    }
    free(message);
    return 0;
}

char* topicID(char* dirname){ //get the id of the person that created a topic
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "TOPICS/%s/%s_UID.txt", dirname, dirname);
    FILE* file;
    size_t len = 0; 
    
    file = fopen(message, "r");
    if(file == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    char *line = NULL;
    getline(&line, &len, file);
    line[strcspn(line, "\n")] = 0;
    free(message);
    return line;
}

void topicList(){ //get the list of topics
    DIR *d;
    struct dirent *dir;
    d = opendir("TOPICS");

    int count = 0;
    if (d){
        dir = readdir(d);
        while(dir != NULL){
            //printf("dirName: %s\n", dir->d_name);
           if ((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                count++;
           		printf("%d - ", count);
                printf("%s (proposed by ", dir->d_name);
                printf("%s)\n", topicID(dir->d_name));
            }
            dir=readdir(d);
        }
        if(closedir(d) == -1){
            printf("Error in receiving information from the server.\n");
            exit(-1);
        }
	}
	else{
		exit(1);
	}
}

void questionList(){
    DIR *d;
    struct dirent *dir;
    char* path = (char*)malloc(sizeof(char)*1024);
    sprintf(path, "TOPICS/%s", local_topic);

    d = opendir(path);
    int count = 0;
    if (d){
        dir = readdir(d);
        while(dir != NULL){
            //printf("dirName: %s\n", dir->d_name);
           if ((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, ".")) && (dir->d_type == DT_DIR)){
                count++;
           		printf("%d - ", count);
                printf("%s (proposed by ", dir->d_name);
                printf("%s) %s\n", questionID(dir->d_name, path), questionAN(dir->d_name, path));

            }
            dir=readdir(d);
        }
        if(closedir(d) == -1){
            printf("Error in receiving information from the server.\n");
            exit(-1);
        }
	}
	else{
        free(path);
		exit(-1);
	}
    free(path);
}

char* questionID(char* dirname, char* path){
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "%s/%s/%s_UID.txt",path, dirname, dirname);
    FILE* file;
    size_t len = 0; 
    
    file = fopen(message, "r");
    char *line = NULL;
    if(file == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    
    getline(&line, &len, file);
    line[strcspn(line, "\n")] = 0;
    free(message);
    return line;
}


char* questionAN(char* dirname, char* path){
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "%s/%s/%s_AN.txt",path, dirname, dirname);
    FILE* file;
    size_t len = 0; 
    
    file = fopen(message, "r");
    char *line = NULL;
    if(file == NULL){
        printf("Error in receiving information from the server.\n");
        exit(-1);
    }
    
    getline(&line, &len, file);
    line[strcspn(line, "\n")] = 0;
    free(message);
    return line;
}



void writeFileData(char* message){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.txt", local_topic, local_question, local_question);
    file = fopen(path, "a");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fprintf(file,"%s", message);
    fclose(file);
    //free(path);
}

void writeFileData2(char* message){
    //DIR *d;
    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);

    sprintf(path, "TOPICS/%s/%s/%s", local_topic, local_question, local_question);
    //d = opendir("path");
    //int fd = dirfd(d);
    //mkdirat(fd, dirname, 0700); o dirname tem que ser o local_question0_0(incrementando)

    file = fopen(path, "a");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fprintf(file,"%s", message);
    fclose(file);
    //free(path);
}

void writeFileImg(char* message, char* ext, long int n){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.%s", local_topic, local_question, local_question, ext);
    file = fopen(path, "ab");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fwrite(message, n, sizeof(char), file);
    fclose(file);
    //free(path);
}

void writeFileImg2(char* message, char* ext, long int n){

    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    memset(path, 0, 1024);
    sprintf(path, "TOPICS/%s/%s/%s.%s", local_topic, local_question, local_question, ext);
    file = fopen(path, "ab");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    fwrite(message, n, sizeof(char), file);
    fclose(file);
    //free(path);
}