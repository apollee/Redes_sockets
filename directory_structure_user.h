#ifndef DIRECTORY_STRUCTURE_USER_H
#define DIRECTORY_STRUCTURE_USER_H 1

int getTopic_by_number(int topic_number);
int checkExistenceofTopic(char* dirname);
char* topicID(char* dirname);
void create_topic_directory(char *dirname, char* userID);
void create_directory(char* dirname);
void topicList();

#endif
