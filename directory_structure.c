#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "directory_structure.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "commands_user.h"

//MAIN FS---------------------------------------------------------
int check_directory_existence(char *dirname){
    DIR *d;
    d = opendir(dirname);

    if(d){
        //printf("directory exists\n");
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

char* topicList(){ //get the list of topics
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
    char* value = malloc(sizeof (char)* 1024);

    d = opendir(path);

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
    return value;
}

void create_topic_directory(char *dirname, char* userID){
    int id = atoi(userID);
    DIR *d;

    //Open topic folder
    d = opendir("TOPICS");
    int fd = dirfd(d);
    mkdirat(fd, dirname, 0700);
    
    //Criar ficheiro
    FILE* file;
    char* path = malloc (sizeof (char) * 1024);
    sprintf(path, "TOPICS/%s/%s_UID.txt", dirname, dirname);
    file = fopen(path, "w");
    if (file < 0) {
        perror("CLIENT:\n");
        exit(1);
    }   
    fprintf(file,"%d", id);
    fclose(file);
    free(path);
    closedir(d);
}

//QUESTION LIST---------------------------------------------------

char* questionList(char* currTopic){
    char* path = malloc(sizeof (char)* 1024);
    DIR *d;
    struct dirent *dir;
    sprintf(path, "TOPICS/%s/", currTopic);
    d = opendir(path);
    char* message = malloc (sizeof (char) * 1024);
    strcpy(message, "");
    char* userID = malloc(sizeof (char)* 1024);
    sprintf(userID, "%s_UID.txt", currTopic);
    if (d){
        while((dir=readdir(d)) != NULL){
            if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, ".")) && (strcmp(dir->d_name, userID))){
                strcat(message, dir->d_name);
                strcat(message, ":");
                strcat(message, questionID(currTopic, dir->d_name));
                strcat(message, ":");
                sprintf(path, "TOPICS/%s/%s/", currTopic, dir->d_name);
                strcat(message, numberOfdirectories(path));
                strcat(message, " ");
            }
        }
        closedir(d);
        return message;
    }
    else 
        return NULL;
}

char* questionID(char* currTopic, char* dirname){
    char* message = malloc (sizeof (char) * 1024); 
    sprintf(message, "TOPICS/%s/%s/%s_UID.txt", currTopic, dirname, dirname);
  
    FILE* file;
    size_t len = 0; 
    
    file = fopen(message, "r");
    char *line = NULL;
    if(file == NULL){
        exit(EXIT_FAILURE);
    }
    
    getline(&line, &len, file);
    line[strcspn(line, "\n")] = 0;
    return line;
}

//USER FUNCTIONS--------------------------------------------------
// int getTopic_by_number(int number){ //get the topic by the number
//     DIR *d;
//     struct dirent *dir;
//     int flag = 0;

//     if(number > 99 && number == 0){
//         return flag;
//     } 

//     d = opendir("TOPICS");
//     int current_topic_number = 1;
//     if(d){
//         while((dir = readdir(d)) != NULL){
//             if((strcmp(dir->d_name, "..")) && (strcmp(dir->d_name, "."))){
//                 if(current_topic_number == number){
//                     strcpy(local_topic, dir->d_name); 
//                     return 1;
//                 }else
//                     current_topic_number++;            
//             }   
//         }
//         closedir(d);
//     }
//     return flag;
// }

// int checkExistenceofTopic(char* dirname){ //check if a topic exists
//     DIR *d;
//     struct dirent *dir;
//     d = opendir("TOPICS");

//     if (d){
//         while((dir=readdir(d)) != NULL){
//             if(!(strcmp(dir->d_name, dirname))){
//                 return 1;
//             }
//         }
//         closedir(d);
//     }
//     return 0;
// }