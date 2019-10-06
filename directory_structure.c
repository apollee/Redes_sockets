#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "directory_structure.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int check_directory_existence(char *dirname){
    DIR *d;
    d = opendir(dirname);

    if(d){
        printf("directory exists\n");
        closedir(d);
        return TRUE;
    }else{
        return FALSE;
    }
}

void create_directory(char *dirname){
    /*DIR *d;
    int success =*/
    mkdir(dirname, 0700);
    /*if(!success){
        printf("created directory.\n");
        d = opendir(dirname);
        return readdir(d);
    }else{
        printf("failed to create directory.\n");
        return NULL;
    }*/
}

char* topicList(){
    DIR *d;
    struct dirent *dir;
    d = opendir("TOPICS");
    char* message = malloc (sizeof (char) * 1024);
    strcpy(message, "");

    if (d){
        while((dir=readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
                strcat(message, dir->d_name);
                strcat(message, ":");
                strcat(message, topicID(dir->d_name));
                strcat(message, " ");
            }
        }
        closedir(d);
        return message;
    }
    else 
        return NULL;
}

char* topicID(char* dirname){
    char* message = malloc (sizeof (char) * 1024);
    strcpy(message, "TOPICS/");
    strcat(message, dirname);
    strcat(message, "/");
    strcat(message, dirname); 
    strcat(message, "_UID.txt");
    FILE* file;
    ssize_t read;
    size_t len = 0; 
    
    file = fopen(message, "r");
    char *line = NULL;
    if(file == NULL){
        exit(EXIT_FAILURE);
    }
    
    read = getline(&line, &len, file);
    line[strcspn(line, "\n")] = 0;
    return line;

}

char* number_of_topics(){
    char* value = malloc(sizeof (char)* 1024);
    int number = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir("TOPICS");
    char* message = malloc (sizeof (char) * 1024);
    strcpy(message, "");

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
    return value;
}
