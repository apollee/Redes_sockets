#ifndef DIRECTORY_STRUCTURE_USER_H
#define DIRECTORY_STRUCTURE_USER_H 1

int getTopic_by_number(int topic_number);
int checkExistenceofTopic(char* dirname);
int checkExistenceofQuestion(char* dirname);
int getQuestion_by_number(int number);
char* topicID(char* dirname);
void create_topic_directory(char *dirname, char* userID);
void create_question_directory(char *dirname, char* userID);
void create_directory(char* dirname);
void topicList();

#endif
