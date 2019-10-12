#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "directory_structure_user.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "commands_user.h"

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