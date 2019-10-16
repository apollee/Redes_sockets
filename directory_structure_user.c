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
    //free(path);
    closedir(d);
}

void create_question_directory(char *dirname, char* userID){
    DIR *d;
    //Open folder of the current topic
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "TOPICS/%s", local_topic);
    d = opendir(message);
    int fd = dirfd(d);
    mkdirat(fd, dirname, 0700);
    
    //Criar ficheiro
    FILE* file;
    char* path = (char*)malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s/%s/%s_UID.txt", local_topic, dirname, dirname);
    file = fopen(path, "w");
    //free(path);
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }
    strcat(userID, "\0");
    fprintf(file,"%s", userID);
    fclose(file);
    //free(path);
    closedir(d);    
}


int getTopic_by_number(int number){ //get the topic by the number
    DIR *d;
    struct dirent *dir;
    int flag = 0;

    if(number > 99 && number == 0){
        return flag;
    } 

    d = opendir("TOPICS");
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
        closedir(d);
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
    int current_question_number = 1;
    if(d){
        while((dir = readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                if(current_question_number == number){
                    strcpy(local_question, dir->d_name); 
                    return 1;
                }else
                    current_question_number++;            
            }   
        }
        closedir(d);
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

int checkExistenceofQuestion(char* dirname){ //check if a topic exists
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
        closedir(d);
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
    char *line = NULL;
    if(file == NULL){
        exit(EXIT_FAILURE);
    }
    
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
    //printf("message antes do topicList: !%s!\n", message);
    if (d){
        dir = readdir(d);
        while(dir != NULL){
            //printf("dirName: %s\n", dir->d_name);
           if ((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                count++;
           		printf("%d - ", count);
                printf("%s (proposed by ", dir->d_name);
                printf("%s)\n", topicID(dir->d_name));
                //if(num != count){
                  //  strcat(message, " ");
                //}
            }
            dir=readdir(d);
        }
        closedir(d);
	}
	else{
		exit(1);
	}
}