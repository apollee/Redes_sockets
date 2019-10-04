#include <dirent.h>

int ListDir(char *dirname){
    Dir *d;
    struct dirent *dir;
    d = opendir(dirname);

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