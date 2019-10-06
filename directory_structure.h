#ifndef DIRECTORY_STRUCTURE_H
#define DIRECTORY_STRUCTURE_H 1

#define FALSE 0
#define TRUE 1

int listDir(char *dirname);
int check_directory_existence(char *dirname);
void create_directory(char *dirname);

#endif