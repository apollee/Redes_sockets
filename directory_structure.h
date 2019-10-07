#ifndef DIRECTORY_STRUCTURE_H
#define DIRECTORY_STRUCTURE_H 1

#define FALSE 0
#define TRUE 1

int check_directory_existence(char *dirname);
void create_directory(char *dirname);
char* topicList();
char* topicID(char* dirname);
char* number_of_topics();
int getTopic_by_number(int topic_number);

#endif