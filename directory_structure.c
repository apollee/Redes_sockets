#include <dirent.h>
#include <stdio.h>
#include "directory_structure.h"
#include <sys/stat.h>
#include <sys/types.h>

int listDir(char *dirname){
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);

    /*searches for stuff inside of TOPICS*/
    if (d){
        while((dir=readdir(d)) != NULL){
            printf("FILE: %s\n", dir->d_name);
        }
        closedir(d);
        return(1);
    }
    else 
        return (-1);
}


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